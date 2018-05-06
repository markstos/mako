#define _POSIX_C_SOURCE 200809L
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

void finish_config(struct mako_config *config) {
	free(config->font);
}

static uint32_t parse_color(const char *color) {
	if (color[0] != '#') {
		return -1;
	}
	color++;

	size_t len = strlen(color);
	if (len != 6 && len != 8) {
		return -1;
	}
	uint32_t res = (uint32_t)strtoul(color, NULL, 16);
	if (len == 6) {
		res = (res << 8) | 0xFF;
	}
	return res;
}

bool parse_config_arguments(struct mako_config *config, int argc, char **argv) {
	static const struct option long_options[] = {
		{"font", required_argument, 0, 0},
		{"background-color", required_argument, 0, 0},
		{"text-color", required_argument, 0, 0},
		{"margin", required_argument, 0, 0},
		{"padding", required_argument, 0, 0},
		{0},
	};

	while (1) {
		int option_index = -1;
		int c = getopt_long(argc, argv, "f", long_options, &option_index);
		if (c < 0) {
			break;
		} else if (c != 0) {
			return false;
		}

		const char *name = long_options[option_index].name;
		if (strcmp(name, "font") == 0) {
			free(config->font);
			config->font = strdup(optarg);
		} else if (strcmp(name, "background-color") == 0) {
			config->colors.background = parse_color(optarg);
		} else if (strcmp(name, "text-color") == 0) {
			config->colors.text = parse_color(optarg);
		} else if (strcmp(name, "margin") == 0) {
			config->margin = strtol(optarg, NULL, 10);
		} else if (strcmp(name, "padding") == 0) {
			config->padding = strtol(optarg, NULL, 10);
		}
	}

	return true;
}
