/*
 * libe cross-platform libraryr: Config
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _CFG_H_
#define _CFG_H_

#include <stdbool.h>


#define CFG_FLAG_CHANGED            0x00000001
#define CFG_FLAG_VOLATILE           0x00000002


int cfg_init(void);
void cfg_quit(void);

/**
 * Make new section to config.
 * @param  section section name
 * @return         0 on success, 1 if section already exists, -1 on errors
 */
int cfg_section_new(char *section);

/**
 * Remove section from configuration.
 * @param  section section name
 * @return         0 on success, -1 if not found
 */
int cfg_section_rm(char *section);

bool cfg_section_exists(char *section);

int cfg_import_file(char *file);
int cfg_export_file(char *file);

/**
 * Free list returned by cfg_list_sections() etc.
 * @param list list returned by cfg_list_sections() etc.
 */
void cfg_list_free(char **list);

/**
 * List all section keys in configuration.
 * @param  count if not NULL, count of keys is stored here
 * @return       list which ends in NULL, must be freed using cfg_list_free()
 */
char **cfg_list_sections(int *count);

/**
 * List all keys for given section in configuration.
 * @param  count if not NULL, count of keys is stored here
 * @return       list, must be freed using cfg_list_free()
 */
char **cfg_list_keys(char *section, int *count);

int cfg_set_str(char *section, char *key, char *value);
int cfg_set_int(char *section, char *key, int value);
int cfg_set_double(char *section, char *key, double value);
int cfg_set_bool(char *section, char *key, bool value);

int cfg_rm(char *section, char *key);

char *cfg_get_str(char *section, char *key, char *_default);
int cfg_get_int(char *section, char *key, int _default);
double cfg_get_double(char *section, char *key, double _default);
bool cfg_get_bool(char *section, char *key, bool _default);

int cfg_flag_is(char *section, char *key, uint32_t flag);
int cfg_flag_set(char *section, char *key, uint32_t flag);
int cfg_flag_clr(char *section, char *key, uint32_t flag);

int cfg_regex_register(char *section_regex, char *key_regex, void (*callback)(char *section, char *key, char *value, void *userdata), void *userdata);


#endif /* _CFG_H_ */
