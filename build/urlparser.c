#define NAPI_VERSION 3

/**#define NAPI_EXPERIMENTAL */

#include <node_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void checkForParameters(napi_env env, char str[], size_t size, napi_value* napi_object){
    napi_value nvalue;
    char path[size];
    char obj_name[size];
    char obj_value[size];
    bool key = false;
    bool value = false;
    bool pathswitch = true;
    int namei = 0;
    int valuei = 0;
    int pathi = 0;

        //walking through str
        while (*str){

            //if ? is found
            if( *str == '?' ){
                key = true;//begin obj_name copy
                pathswitch = false;//path copy ends
                path[pathi] = '\0';//ending path as a null-terminated string
                str++;//skeeping char '?'
            }
            
            //if = is found
            else if( *str == '=' ){
                value = true;//begin obj_value copy
                key = false; //obj_name copy ends
                str++;//skeeping char '='
            }

            //if & is found
            else if(*str == '&' ){
                //convert arrays to null-terminated strings
                obj_name[namei] = '\0';
                obj_value[valuei] = '\0';

                //grab their values here
                napi_create_string_utf8(env, (char*) &obj_value, valuei, &nvalue);
                napi_set_named_property(env, *napi_object, obj_name, nvalue);
                
                //reset obj_name and obj_value
                key = true;
                value = false;
                namei = 0;
                valuei = 0;

                str++;//skeeping & char
            }


            if(pathswitch == true){
                //copying path
                *(&path[pathi]) = *str;
                pathi++;
            }

            else if (key == true)
            {
                //copying object name into obj_name
                *(&obj_name[namei]) = *str;
                namei++;
            }

            else if(value == true){
                //copying object value into obj_value
                obj_name[namei] = '\0'; //closing obj_name copy
                *(&obj_value[valuei]) = *str;
                valuei++;
            }

            str++;
            
        } 

    obj_value[valuei] = '\0';

    napi_create_string_utf8(env, (char*) &obj_value, valuei, &nvalue);
    napi_set_named_property(env, *napi_object, obj_name, nvalue);    

    napi_create_string_utf8(env, (char*) &path, pathi, &nvalue);
    napi_set_named_property(env, *napi_object, "path", nvalue);

}


napi_value parse(napi_env env, napi_callback_info info){
    napi_value argv[1];
    napi_value myobject;
    
    size_t argc = 1;

    napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
    if (argc < 1) {
        napi_throw_error(env, "EINVAL", "Too few arguments");
        return NULL;
    }


    napi_get_value_string_utf8(env, argv[0], NULL, 0, &argc);
    

    char str[argc+1];

    napi_get_value_string_utf8(
          env          //environment
        , argv[0]      //napi_value representa un string en javascript
        , (char*) &str //(c/c++)Buffer para recibir el string utf8
        , sizeof(str)  //Tamaño del buffer destino
        , &argc        // numero de bytes copiados al buffer destino
    );

    
    napi_create_object(env, &myobject);//creating the napi object

    checkForParameters(env, str, argc, &myobject);

    return myobject;
}

napi_value init(napi_env env, napi_value exports){
    napi_value parse_fn;

    napi_create_function(env, NULL, 0, parse, NULL, &parse_fn);
    napi_set_named_property(env, exports, "parse", parse_fn);

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init);

