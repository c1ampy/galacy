#include "file_io.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 文件名常量
#define HIGH_SCORE_FILE "high_score.dat"
#define CONFIG_FILE      "config.dat"
#define SAVE_FILE_PREFIX "save_"
#define SAVE_FILE_SUFFIX ".dat"

// 保存最高分
FileIOStatus file_io_save_high_score(uint32_t high_score) {
    FILE *file = fopen(HIGH_SCORE_FILE, "wb");
    if (file == NULL) {
        return FILE_IO_ERROR_OPEN;
    }
    
    size_t written = fwrite(&high_score, sizeof(uint32_t), 1, file);
    fclose(file);
    
    return (written == 1) ? FILE_IO_SUCCESS : FILE_IO_ERROR_WRITE;
}

// 读取最高分
FileIOStatus file_io_load_high_score(uint32_t *high_score) {
    if (high_score == NULL) {
        return FILE_IO_ERROR_FORMAT;
    }
    
    FILE *file = fopen(HIGH_SCORE_FILE, "rb");
    if (file == NULL) {
        *high_score = 0; // 默认值
        return FILE_IO_SUCCESS;
    }
    
    size_t read = fread(high_score, sizeof(uint32_t), 1, file);
    fclose(file);
    
    if (read != 1) {
        *high_score = 0;
        return FILE_IO_ERROR_READ;
    }
    
    return FILE_IO_SUCCESS;
}

// 保存游戏配置
FileIOStatus file_io_save_config(const GameConfig *config) {
    if (config == NULL) {
        return FILE_IO_ERROR_FORMAT;
    }
    
    FILE *file = fopen(CONFIG_FILE, "wb");
    if (file == NULL) {
        return FILE_IO_ERROR_OPEN;
    }
    
    size_t written = fwrite(config, sizeof(GameConfig), 1, file);
    fclose(file);
    
    return (written == 1) ? FILE_IO_SUCCESS : FILE_IO_ERROR_WRITE;
}

// 加载游戏配置
FileIOStatus file_io_load_config(GameConfig *config) {
    if (config == NULL) {
        return FILE_IO_ERROR_FORMAT;
    }
    
    FILE *file = fopen(CONFIG_FILE, "rb");
    if (file == NULL) {
        file_io_get_default_config(config); // 使用默认配置
        return FILE_IO_SUCCESS;
    }
    
    size_t read = fread(config, sizeof(GameConfig), 1, file);
    fclose(file);
    
    if (read != 1) {
        file_io_get_default_config(config);
        return FILE_IO_ERROR_READ;
    }
    
    return FILE_IO_SUCCESS;
}

// 保存游戏存档
FileIOStatus file_io_save_game(const char *save_name, const GameSave *save_data) {
    if (save_name == NULL || save_data == NULL) {
        return FILE_IO_ERROR_FORMAT;
    }
    
    // 构建存档文件名
    char filename[50];
    snprintf(filename, sizeof(filename), "%s%s%s", SAVE_FILE_PREFIX, save_name, SAVE_FILE_SUFFIX);
    
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        return FILE_IO_ERROR_OPEN;
    }
    
    size_t written = fwrite(save_data, sizeof(GameSave), 1, file);
    fclose(file);
    
    return (written == 1) ? FILE_IO_SUCCESS : FILE_IO_ERROR_WRITE;
}

// 加载游戏存档
FileIOStatus file_io_load_game(const char *save_name, GameSave *save_data) {
    if (save_name == NULL || save_data == NULL) {
        return FILE_IO_ERROR_FORMAT;
    }
    
    // 构建存档文件名
    char filename[50];
    snprintf(filename, sizeof(filename), "%s%s%s", SAVE_FILE_PREFIX, save_name, SAVE_FILE_SUFFIX);
    
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return FILE_IO_ERROR_OPEN;
    }
    
    size_t read = fread(save_data, sizeof(GameSave), 1, file);
    fclose(file);
    
    return (read == 1) ? FILE_IO_SUCCESS : FILE_IO_ERROR_READ;
}

// 删除游戏存档
FileIOStatus file_io_delete_game(const char *save_name) {
    if (save_name == NULL) {
        return FILE_IO_ERROR_FORMAT;
    }
    
    // 构建存档文件名
    char filename[50];
    snprintf(filename, sizeof(filename), "%s%s%s", SAVE_FILE_PREFIX, save_name, SAVE_FILE_SUFFIX);
    
    // 尝试删除文件
    if (remove(filename) != 0) {
        return FILE_IO_ERROR_OPEN; // 文件不存在或删除失败
    }
    
    return FILE_IO_SUCCESS;
}

// 获取默认游戏配置
void file_io_get_default_config(GameConfig *config) {
    if (config != NULL) {
        config->difficulty = 1;      // 默认简单难度
        config->sound_volume = 80;   // 默认声音音量80%
        config->music_volume = 70;   // 默认音乐音量70%
        config->show_fps = 0;        // 默认不显示FPS
    }
}