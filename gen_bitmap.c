#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_INDEX 0x10000  // 最大索引值 65535
#define BITMAP_SIZE ((MAX_INDEX / 8) + 1)  // 固定位图大小

// 安全解析十六进制字符串（支持最多8位数字）
bool parse_hex_string(const char *str, uint32_t *result) {
    char hex_str[9] = {0};  // 8字符+终止符
    size_t len = 0;
    
    // 跳过前导0并提取有效十六进制字符
    while (*str == '0') str++;  // 跳过前导零
    while (isxdigit((unsigned char)*str) && len < 8) {
        hex_str[len++] = *str++;
    }
    hex_str[len] = '\0';
    
    if (len == 0) return false;
    
    // 转换为32位整数
    char *endptr;
    unsigned long value = strtoul(hex_str, &endptr, 16);
    
    // 检查转换是否有效且不越界
    if (*endptr != '\0' || value > MAX_INDEX) {
        return false;
    }
    
    *result = (uint32_t)value;
    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // 初始化位图（固定大小）
    uint8_t bitmap[BITMAP_SIZE] = {0};
    uint32_t max_used_index = 0;

    // 处理输入文件
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("File open failed");
        return EXIT_FAILURE;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        char *ptr = buffer;
        while (*ptr) {
            if (strncmp(ptr, "0x", 2) == 0) {
                ptr += 2;  // 跳过"0x"
                
                // 解析并设置位图
                uint32_t index;
                if (parse_hex_string(ptr, &index)) {
                    // 更新最大索引（优化输出）
                    if (index > max_used_index) max_used_index = index;
                    
                    // 位图置位操作
                    size_t byte_idx = index / 8;
                    uint8_t bit_offset = index % 8;
                    bitmap[byte_idx] |= (1 << bit_offset);
                }
                
                // 跳过已处理字符
                while (isxdigit((unsigned char)*ptr)) ptr++;
            } else {
                ptr++;
            }
        }
    }
    fclose(file);

    // 计算实际使用的位图大小（优化输出体积）
    size_t output_size = (max_used_index / 8) + 1;
    
    // 输出位图表达式
    printf("u8 passthrough_msrs_bitmap[] = {\n");
    for (size_t i = 0; i < output_size; i++) {
        // 生成二进制格式字符串
        char bin_str[11] = "0b00000000";
        for (int j = 0; j < 8; j++) {
            if (bitmap[i] & (1 << j)) {
                bin_str[9 - j] = '1';  // 高位在前
            }
        }
        
        // 每行输出4个元素
        printf("    %s%s", bin_str, 
              (i == output_size - 1) ? "" : ",");
        if ((i + 1) % 4 == 0 || i == output_size - 1) 
            printf("  // 0x%04zX\n", i * 8);
    }
    printf("};\n");

    return EXIT_SUCCESS;
}

