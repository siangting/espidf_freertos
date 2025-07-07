# components/freertos/CMakeLists.txt

# 自動抓取所有核心與 port 層的 C 檔
file(GLOB freertos_src
    "src/*.c"
    "portable/ThirdParty/GCC/Xtensa_ESP32/*.c"
)

# 告訴 IDF build system 這是一個 component
idf_component_register(
    SRCS ${freertos_src}
    INCLUDE_DIRS include
    # 如果你的任務要用到 esp_timer，就保留這一行；否則移除
    REQUIRES esp_timer
)
