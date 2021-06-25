#include "utils.h"

int get_node_data_type(node * node_param){
    switch(node_param->type){
        case VARIABLE:  
            return ((variable_node *) (node_param))->variable_type;
        case CONSTANT_STRING:   
            return STRING_TYPE;
        case CONSTANT_INT:  
            return INT_TYPE;
        case EXP:  
            return INT_TYPE;
        case PROPERTY_NODE:
            return ((property_node *)(node_param))->return_type;
        case FUNCTION_NODE:
            return ((function_node *)(node_param))->return_type;
        default:    
            return -1;
    }
    return -1;

}

//on this instance, all properties are INT_TYPE, but this could change as new properties are created
data_type get_property_return_type(figure_property_type type){
    data_type ret;
    switch(type){
        case RADIUS_TYPE:
        case SIDE_1_TYPE:
        case SIDE_2_TYPE:
        case SIDE_3_TYPE:
        case BASE_TYPE:
        case HEIGHT_TYPE:
            ret = INT_TYPE;
    }
    return ret;
}
