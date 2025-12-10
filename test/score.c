#include "score.h"
#include <string.h>
#include <stdio.h>

// 静态全局变量
static uint32_t current_score = 0;
static RankingEntry rankings[MAX_RANKINGS];
static uint8_t ranking_count = 0;

// 分数系统初始化
void score_init(void) {
    current_score = 0;
    ranking_count = 0;
    memset(rankings, 0, sizeof(rankings));
    score_load_rankings(); // 从文件加载排行榜
}

// 获取当前分数
uint32_t score_get_current(void) {
    return current_score;
}

// 增加分数
void score_add(uint32_t points) {
    current_score += points;
}

// 重置分数
void score_reset(void) {
    current_score = 0;
}

// 获取排行榜
void score_get_rankings(RankingEntry rankings[], uint8_t *count) {
    if (rankings != NULL && count != NULL) {
        memcpy(rankings, ::rankings, sizeof(RankingEntry) * ranking_count);
        *count = ranking_count;
    }
}

// 比较函数，用于排行榜排序
static int compare_rankings(const void *a, const void *b) {
    const RankingEntry *entry1 = (const RankingEntry *)a;
    const RankingEntry *entry2 = (const RankingEntry *)b;
    // 按分数降序排列
    if (entry1->score < entry2->score) {
        return 1;
    } else if (entry1->score > entry2->score) {
        return -1;
    } else {
        return 0;
    }
}

// 更新排行榜（添加新分数）
void score_update_ranking(const char *name, uint32_t score) {
    // 检查是否可以添加到排行榜
    if (ranking_count < MAX_RANKINGS || score > rankings[MAX_RANKINGS - 1].score) {
        // 添加新条目
        if (ranking_count < MAX_RANKINGS) {
            ranking_count++;
        }
        
        // 复制名称和分数
        strncpy(rankings[ranking_count - 1].name, name, sizeof(rankings[0].name) - 1);
        rankings[ranking_count - 1].name[sizeof(rankings[0].name) - 1] = '\0';
        rankings[ranking_count - 1].score = score;
        
        // 排序排行榜
        qsort(rankings, ranking_count, sizeof(RankingEntry), compare_rankings);
        
        // 保存到文件
        score_save_rankings();
    }
}

// 保存排行榜到文件
void score_save_rankings(void) {
    FILE *file = fopen("rankings.dat", "wb");
    if (file != NULL) {
        fwrite(&ranking_count, sizeof(uint8_t), 1, file);
        fwrite(rankings, sizeof(RankingEntry), ranking_count, file);
        fclose(file);
    }
}

// 从文件加载排行榜
void score_load_rankings(void) {
    FILE *file = fopen("rankings.dat", "rb");
    if (file != NULL) {
        fread(&ranking_count, sizeof(uint8_t), 1, file);
        if (ranking_count > MAX_RANKINGS) {
            ranking_count = MAX_RANKINGS;
        }
        fread(rankings, sizeof(RankingEntry), ranking_count, file);
        fclose(file);
    }
}