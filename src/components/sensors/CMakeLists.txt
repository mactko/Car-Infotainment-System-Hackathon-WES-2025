idf_component_register(
    SRCS "sensors.c"
    INCLUDE_DIRS "."
    REQUIRES driver veml7700 hc-sr04 sht31 tcrt5000 sensors gui freertos buzzer radio gui
)