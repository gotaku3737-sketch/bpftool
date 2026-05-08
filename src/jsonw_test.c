#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "json_writer.h"

static void test_pretty_off()
{
	char *buf = NULL;
	size_t len = 0;
	FILE *f = open_memstream(&buf, &len);
	assert(f);

	json_writer_t *wr = jsonw_new(f);
	jsonw_pretty(wr, false);
	jsonw_start_object(wr);
	jsonw_string_field(wr, "key", "value");
	jsonw_end_object(wr);
	jsonw_destroy(&wr);

	fclose(f);
	/* jsonw_destroy adds a \n at the end */
	assert(strcmp(buf, "{\"key\":\"value\"}\n") == 0);
	free(buf);
}

static void test_pretty_on()
{
	char *buf = NULL;
	size_t len = 0;
	FILE *f = open_memstream(&buf, &len);
	assert(f);

	json_writer_t *wr = jsonw_new(f);
	jsonw_pretty(wr, true);
	jsonw_start_object(wr);
	jsonw_string_field(wr, "key", "value");
	jsonw_end_object(wr);
	jsonw_destroy(&wr);

	fclose(f);
	/* Expected output with pretty:
	 * {
	 *     "key": "value"
	 * }
	 */
	const char *expected = "{\n    \"key\": \"value\"\n}\n";
	assert(strcmp(buf, expected) == 0);
	free(buf);
}

static void test_pretty_toggle()
{
	char *buf = NULL;
	size_t len = 0;
	FILE *f = open_memstream(&buf, &len);
	assert(f);

	json_writer_t *wr = jsonw_new(f);
	jsonw_start_object(wr);
	jsonw_pretty(wr, true);
	jsonw_name(wr, "pretty");
	jsonw_start_object(wr);
	jsonw_string_field(wr, "a", "b");
	jsonw_end_object(wr);

	jsonw_pretty(wr, false);
	jsonw_name(wr, "ugly");
	jsonw_start_object(wr);
	jsonw_string_field(wr, "c", "d");
	jsonw_end_object(wr);

	jsonw_end_object(wr);
	jsonw_destroy(&wr);

	fclose(f);

	/* {
	 *     "pretty": {
	 *         "a": "b"
	 *     },"ugly":{"c":"d"}}
	 */
	const char *expected = "{\n    \"pretty\": {\n        \"a\": \"b\"\n    },\"ugly\":{\"c\":\"d\"}}\n";
	assert(strcmp(buf, expected) == 0);
	free(buf);
}

int main()
{
	test_pretty_off();
	test_pretty_on();
	test_pretty_toggle();
	printf("All tests passed!\n");
	return 0;
}
