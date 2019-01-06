/*
 * libe cross-platform library: Config
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <stdio.h>
#include <stdint.h>
#include <pcre.h>
#include <libe/debug.h>
#include <libe/linkedlist.h>
#include "cfg.h"


struct cfg_item {
	char *key;
	char *value;
	uint32_t flags;
	struct cfg_item *prev;
	struct cfg_item *next;
};

struct cfg_section {
	char *key;
	struct cfg_item *first;
	struct cfg_item *last;
	struct cfg_section *prev;
	struct cfg_section *next;
};

struct cfg_regex {
	pcre *section;
	pcre *key;
	void (*callback)(char *section, char *key, char *value, void *userdata);
	void *userdata;
	struct cfg_regex *prev;
	struct cfg_regex *next;
};

/* first section */
struct cfg_section *cfg_sf = NULL;
/* last section */
struct cfg_section *cfg_sl = NULL;
/* first regex */
struct cfg_regex *cfg_regex_first = NULL;
/* last regex */
struct cfg_regex *cfg_regex_last = NULL;

/* shorthand macro for finding item by string key from list */
#define CFG_FIND(first, last, item, name) \
do { \
	item = first; \
	while (item) { \
		if (strcmp(item->key, name) == 0) { \
			break; \
		} \
		item = item->next; \
	} \
} while (0)


/**
 * Strip white-spaces from the beginning and end of a string.
 * This function returns pointer to first non white-space character and sets
 * null character at first non white-space looking from end of the string.
 * If string contains only white-spaces, pointer returned is set at terminating
 * null character of the string.
 *
 * @param str string to be handled
 * @return    pointer to first non white-space character
 */
char *_v_trim(char *str)
{
	char *p;

	/* find first non white-space */
	for ( ; isspace((int)(*str)); str++);
	/* check if the string contains only white-spaces */
	if (*str == '\0') return str;

	/* find last non white-space */
	for (p = str + strlen(str) - 1; isspace((int)(*p)); p--);
	p[1] = '\0';

	return str;
}


int cfg_init(void)
{
	cfg_sf = NULL;
	cfg_sl = NULL;
	cfg_regex_first = NULL;
	cfg_regex_last = NULL;
	return 0;
}


void cfg_quit(void)
{
	struct cfg_section *sct;
	struct cfg_regex *regex;

	for (sct = cfg_sf; sct; ) {
		char *section = sct->key;
		sct = sct->next;
		cfg_section_rm(section);
	};

	for (regex = cfg_regex_first; regex; ) {
		struct cfg_regex *temp = regex->next;
		pcre_free(regex->section);
		pcre_free(regex->key);
		free(regex);
		regex = temp;
	};
}


int cfg_section_new(char *section)
{
	struct cfg_section *sct;

	CFG_FIND(cfg_sf, cfg_sl, sct, section);
	if (sct) {
		/* section already exists */
		return 1;
	}
	/* setup new section */
	SALLOC(sct, -1);
	sct->key = strdup(section);
	LL_APP(cfg_sf, cfg_sl, sct);

	return 0;
}


int cfg_section_rm(char *section)
{
	struct cfg_section *sct;
	struct cfg_item *item;

	CFG_FIND(cfg_sf, cfg_sl, sct, section);
	ERROR_IF_R(!sct, -1, "trying to remove non-existent section: %s", section);
	/* free resources */
	item = sct->first;
	while (item) {
		char *key = item->key;
		item = item->next;
		cfg_rm(section, key);
	};
	LL_RM(cfg_sf, cfg_sl, sct);
	free(sct->key);
	free(sct);

	return 0;
}


bool cfg_section_exists(char *section)
{
	struct cfg_section *sct;
	CFG_FIND(cfg_sf, cfg_sl, sct, section);
	if (sct) {
		return 1;
	}
	return 0;
}


int cfg_import_file(char *file)
{
	FILE *f;
	char *line = NULL, *cur, *value, *middle;
	char *section = NULL, *key = NULL;
	size_t n = 0;
	int err = 0, i;

	/* open file for reading */
	f = fopen(file, "r");
	CRIT_IF_R(!f, -1, "failed to open configuration file: %s", file);

	for (line = NULL, n = 0, i = 1; ; i++) {
		/* get new line from stream */
		err = getline(&line, &n, f);
		if (err < 1) {
			break;
		}
		/* trim line */
		cur = _v_trim(line);
		if (strlen(cur) < 1) {
			continue;
		}
		/* dont parse commented lines */
		if (*cur == '#' || *cur == ';') {
			continue;
		}

		/* parse new list */
		if (*cur == '[') {
			char ce;
			if (section) {
				free(section);
			}
			err = sscanf(cur, "[%m[^]\n]%c", &section, &ce);
			if (err == 2 && ce == ']') {
				cfg_section_new(section);
				// DEBUG_MSG("section: %s", section);
			} else {
				ERROR_MSG("invalid section line %d: %s", i, cur);
			}
			continue;
		}

		/* parse variable */
		key = NULL; middle = NULL; value = NULL;
		err = sscanf(cur, "%m[^ =\t\n]%m[ =\t]%m[^\n]", &key, &middle, &value);
		if (err == 3 && strchr(middle, (int)'=')) {
			char *value_trimmed = _v_trim(value);
			cfg_set_str(section, key, value_trimmed);
			// DEBUG_MSG(" - %s = %s", key, value_trimmed);
		} else {
			ERROR_MSG("invalid configuration line %d: %s", i, cur);
		}

		if (key) {
			free(key);
		}
		if (middle) {
			free(middle);
		}
		if (value) {
			free(value);
		}
	}

	fclose(f);
	if (section) {
		free(section);
	}
	if (line) {
		free(line);
	}

	return 0;
}


int cfg_export_file(char *file)
{
	FILE *f;
	struct cfg_section *sct;
	struct cfg_item *item;

	/* open file for writing */
	f = fopen(file, "w");
	CRIT_IF_R(!f, -1, "failed to open configuration file: %s", file);
	/* write contents */
	for (sct = cfg_sf; sct; sct = sct->next) {
		fprintf(f, "[%s]\n", sct->key);
		for (item = sct->first; item; item = item->next) {
			if (!item->value) {
				/* skip items that dont have a value set */
				continue;
			}
			if (item->flags & CFG_FLAG_VOLATILE) {
				/* skip items that should not be saved */
				continue;
			}
			fprintf(f, "%s = %s\n", item->key, item->value);
		}
	};
	fclose(f);

	return 0;
}


void cfg_list_free(char **list)
{
	int i;
	for (i = 0; list[i]; i++) {
		free(list[i]);
	}
	free(list);
}


char **cfg_list_sections(int *count)
{
	struct cfg_section *sct;
	char **sections;
	int c, i;

	/* count sections */
	LL_COUNT(cfg_sf, cfg_sl, c);
	if (count) {
		*count = c;
	}

	/* allocate memory for pointers + 1, the last one is always NULL */
	sections = (char **)malloc(sizeof(*sections) * (c + 1));
	CRIT_IF_R(!sections, NULL, "malloc(sections) failed");
	memset(sections, 0, sizeof(*sections) * (c + 1));

	/* copy section keys */
	for (sct = cfg_sf, i = 0; sct; sct = sct->next, i++) {
		sections[i] = strdup(sct->key);
		CRIT_IF_R(!sections[i], NULL, "strdup(section) failed");
	}

	return sections;
}


char **cfg_list_keys(char *section, int *count)
{
	struct cfg_section *sct;
	struct cfg_item *item;
	char **keys;
	int c, i;

	/* find section */
	CFG_FIND(cfg_sf, cfg_sl, sct, section);
	if (!sct) {
		/* section not found, return empty key list */
		keys = (char **)malloc(sizeof(*keys));
		memset(keys, 0, sizeof(*keys));
		if (count) {
			*count = 0;
		}
		ERROR_MSG("section not found, section: %s", section);
		return keys;
	}

	/* count keys */
	LL_COUNT(sct->first, sct->last, c);
	if (count) {
		*count = c;
	}

	/* allocate memory for pointers + 1, the last one is always NULL */
	keys = (char **)malloc(sizeof(*keys) * (c + 1));
	CRIT_IF_R(!keys, NULL, "malloc(keys) failed");
	memset(keys, 0, sizeof(*keys) * (c + 1));

	/* copy keys */
	for (item = sct->first, i = 0; item; item = item->next, i++) {
		keys[i] = strdup(item->key);
		CRIT_IF_R(!keys[i], NULL, "strdup(key) failed");
	}

	return keys;
}


int cfg_set_str(char *section, char *key, char *value)
{
	struct cfg_section *sct;
	struct cfg_item *item;
	struct cfg_regex *regex;
	char *value_trimmed;

	/* find section */
	CFG_FIND(cfg_sf, cfg_sl, sct, section);
	ERROR_IF_R(!sct, -1, "section not found when trying to set value, section: %s, key: %s", section, key);

	/* find if item exists */
	CFG_FIND(sct->first, sct->last, item, key);
	if (!item) {
		/* item does not exists, allocate new */
		SALLOC(item, -1);
		item->key = strdup(key);
		LL_APP(sct->first, sct->last, item);
	}

	/* only set new value if it is not NULL,
	 * otherwise this function just made sure this value existed
	 */
	if (!value) {
		return 0;
	}

	/* trim value */
	value = strdup(value);
	value_trimmed = _v_trim(value);

	/* setup item value */
	if (!item->value) {
		/* new value */
		item->value = strdup(value_trimmed);
		item->flags |= CFG_FLAG_CHANGED;
	} else if (strcmp(item->value, value) != 0) {
		/* old value changed */
		free(item->value);
		item->value = strdup(value_trimmed);
		item->flags |= CFG_FLAG_CHANGED;
	}

	/* this is duplicate of original, free it */
	free(value);

	if (!(item->flags & CFG_FLAG_CHANGED)) {
		return 0;
	}

	/* check for regex matches */
	for (regex = cfg_regex_first; regex; regex = regex->next) {
		int nomatch = pcre_exec(regex->section, NULL, section, strlen(section), 0, 0, NULL, 0);
		if (nomatch) {
			continue;
		}
		if (regex->key) {
			nomatch = pcre_exec(regex->key, NULL, key, strlen(key), 0, 0, NULL, 0);
		} else {
			nomatch = 0;
		}
		if (!nomatch) {
			regex->callback(section, key, item->value, regex->userdata);
		}
	}

	return 0;
}


int cfg_set_int(char *section, char *key, int value)
{
	int size;
	char *value_str = NULL;

	/* format given number as ascii data */
	size = asprintf(&value_str, "%d", value);
	ERROR_IF_R(!value_str || size < 1, -1, "asprintf(double) failed");
	/* set value */
	cfg_set_str(section, key, value_str);
	free(value_str);

	return 0;
}


int cfg_set_double(char *section, char *key, double value)
{
	int size;
	char *value_str = NULL;

	/* format given number as ascii data */
	size = asprintf(&value_str, "%.17g", value);
	ERROR_IF_R(!value_str || size < 1, -1, "asprintf(double) failed");
	/* set value */
	cfg_set_str(section, key, value_str);
	free(value_str);

	return 0;
}


int cfg_set_bool(char *section, char *key, bool value)
{
	return cfg_set_int(section, key, value ? 1 : 0);
}


int cfg_rm(char *section, char *key)
{
	struct cfg_section *sct;
	struct cfg_item *item;

	/* find section */
	CFG_FIND(cfg_sf, cfg_sl, sct, section);
	ERROR_IF_R(!sct, -1, "trying to remove item from non-existent section: %s", section);
	/* find item and remove it */
	CFG_FIND(sct->first, sct->last, item, key);
	ERROR_IF_R(!item, -1, "trying to remove non-existent item, section: %s, key: %s", section, key);
	LL_RM(sct->first, sct->last, item);
	/* free resources */
	free(item->key);
	free(item->value);
	free(item);

	return 0;
}


char *cfg_get_str(char *section, char *key, char *_default)
{
	struct cfg_section *sct;
	struct cfg_item *item;

	/* find section */
	CFG_FIND(cfg_sf, cfg_sl, sct, section);
	if (!sct) {
		return _default;
	}
	/* find item */
	CFG_FIND(sct->first, sct->last, item, key);
	if (!item) {
		return _default;
	}

	return item->value;
}


int cfg_get_int(char *section, char *key, int _default)
{
	char *value = cfg_get_str(section, key, NULL);
	if (!value) {
		/* value not found, return default */
		return _default;
	}
	if (!isdigit(*value)) {
		/* value does not start with digit, return default */
		return _default;
	}
	return atoi(value);
}


double cfg_get_double(char *section, char *key, double _default)
{
	char *value = cfg_get_str(section, key, NULL);
	if (!value) {
		/* value not found, return default */
		return _default;
	}
	if (!isdigit(*value)) {
		/* value does not start with digit, return default */
		return _default;
	}
	return atof(value);
}


bool cfg_get_bool(char *section, char *key, bool _default)
{
	return cfg_get_int(section, key, _default ? 1 : 0) ? true : false;
}


int cfg_flag_is(char *section, char *key, uint32_t flag)
{
	struct cfg_section *sct;
	struct cfg_item *item;

	/* find section */
	CFG_FIND(cfg_sf, cfg_sl, sct, section);
	if (!sct) {
		return 0;
	}
	/* find item */
	CFG_FIND(sct->first, sct->last, item, key);
	if (!item) {
		return 0;
	}

	return (item->flags & flag) ? 1 : 0;
}


int cfg_flag_set(char *section, char *key, uint32_t flag)
{
	struct cfg_section *sct;
	struct cfg_item *item;

	/* make sure section and item with this key exists */
	cfg_section_new(section);
	cfg_set_str(section, key, NULL);

	/* find section */
	CFG_FIND(cfg_sf, cfg_sl, sct, section);
	if (!sct) {
		return -1;
	}
	/* find item */
	CFG_FIND(sct->first, sct->last, item, key);
	if (!item) {
		return -1;
	}

	item->flags |= flag;

	return 0;
}


int cfg_flag_clr(char *section, char *key, uint32_t flag)
{
	struct cfg_section *sct;
	struct cfg_item *item;

	/* make sure section and item with this key exists */
	cfg_section_new(section);
	cfg_set_str(section, key, NULL);

	/* find section */
	CFG_FIND(cfg_sf, cfg_sl, sct, section);
	if (!sct) {
		return -1;
	}
	/* find item */
	CFG_FIND(sct->first, sct->last, item, key);
	if (!item) {
		return -1;
	}

	item->flags &= ~flag;

	return 0;
}


int cfg_regex_register(char *section_regex, char *key_regex, void (*callback)(char *section, char *key, char *value, void *userdata), void *userdata)
{
	struct cfg_regex *regex;
	const char *errmsg;
	int erroff;

	/* base setup */
	SALLOC(regex, -1);
	regex->callback = callback;
	regex->userdata = userdata;

	/* compile both expressions */
	regex->section = pcre_compile(section_regex, 0, &errmsg, &erroff, NULL);
	ERROR_IF_R(!regex->section, -1, "invalid section regular expression '%s': %s", section_regex, errmsg);
	if (key_regex) {
		regex->key = pcre_compile(key_regex, 0, &errmsg, &erroff , NULL);
		ERROR_IF_R(!regex->key, -1, "invalid key regular expression '%s': %s", key_regex, errmsg);
	}

	/* append to list */
	LL_APP(cfg_regex_first, cfg_regex_last, regex);

	return 0;
}

