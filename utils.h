#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdbool.h>
#include "node_definitions.h"

//Returns enum of data type wrapped inside of node
int get_node_data_type(node * node_param);

//Returns data type of figure property (eg. for radius, returns int)
data_type get_property_return_type(figure_property_type type);

//Returns if a data type represents a figure
bool is_type_figure(int type);

#endif
