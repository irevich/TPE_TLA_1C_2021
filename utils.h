#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdbool.h>
#include "node_definitions.h"

int get_node_data_type(node * node_param);

data_type get_property_return_type(figure_property_type type);

bool is_type_figure(int type);

#endif
