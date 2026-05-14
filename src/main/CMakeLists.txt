set(COMPONENT_SRCS "app_main.c")
set(COMPONENT_ADD_INCLUDEDIRS "")
set(REQUIRES driver veml7700 hc-sr04 sht31 tcrt5000 sensors gui)

register_component()
