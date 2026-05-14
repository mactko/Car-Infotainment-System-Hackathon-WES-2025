idf_component_register(SRCS "hc-sr04.c"
                    PRIV_REQUIRES 
                    INCLUDE_DIRS "."
                    REQUIRES driver esp_timer)