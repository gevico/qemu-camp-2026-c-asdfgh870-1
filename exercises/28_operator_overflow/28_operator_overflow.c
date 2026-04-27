#include <stdio.h>
#include <limits.h>

#define CHECK_OVERFLOW(carry) \
    carry ? "Overflow" : "Not Overflow"

int check_add_overflow_asm(unsigned int a, unsigned int b) {
    unsigned char carry;
    __asm__ volatile(
        "add %2, %1\n\t"       // eax = a + b
        "setc %0"              // 有进位 CF=1 → carry=1（溢出）
        : "=r"(carry)
        : "a"(a), "r"(b)
        : "cc"
    );
    return carry;
}

int check_sub_overflow_asm(unsigned int a, unsigned int b) {
    unsigned char carry;
    __asm__ volatile(
        "sub %2, %1\n\t"       // eax = a - b
        "setc %0"              // 有借位 CF=1 → 溢出
        : "=r"(carry)
        : "a"(a), "r"(b)
        : "cc"
    );
    return carry;
}

int check_mul_overflow_asm(unsigned int a, unsigned int b) {
    unsigned int high_bits;
    __asm__ volatile(
        "xor %%edx, %%edx\n\t" // 清空 edx
        "mul %2\n\t"           // eax * b → edx:eax
        "mov %%edx, %0"        // 高32位存入 high_bits
        : "=r"(high_bits)
        : "a"(a), "r"(b)
        : "edx", "cc"
    );
    return  (high_bits != 0);
}

int check_div_overflow_asm(unsigned int a, unsigned int b) {
    unsigned char is_div_zero;
    __asm__ volatile(
        "test %1, %1\n\t"      // 判断 b 是否为 0
        "setz %0"              // 为 0 → 设置 1
        : "=r"(is_div_zero)
        : "r"(b)
        : "cc"
    );
    return is_div_zero;
}

int main() {
    printf("(UINT_MAX + 1)Add: %s\n", CHECK_OVERFLOW(check_add_overflow_asm(UINT_MAX, 1)));   // 1
    printf("(1, 0)Add: %s\n", CHECK_OVERFLOW(check_add_overflow_asm(1, 0)));  
    printf("(0, 1)Sub: %s\n", CHECK_OVERFLOW(check_sub_overflow_asm(0, 1)));          // 1
    printf("(2, 1)Sub: %s\n", CHECK_OVERFLOW(check_sub_overflow_asm(2, 1)));
    printf("(UINT_MAX, 2)Mul: %s\n", CHECK_OVERFLOW(check_mul_overflow_asm(UINT_MAX, 2)));   // 1
    printf("(1, 2)Mul: %s\n", CHECK_OVERFLOW(check_mul_overflow_asm(1, 2)));
    printf("(10, 0)Div: %s\n", CHECK_OVERFLOW(check_div_overflow_asm(10, 0)));                         // 1
    printf("(2, 1)Div: %s\n", CHECK_OVERFLOW(check_div_overflow_asm(2, 1)));
    return 0;
}