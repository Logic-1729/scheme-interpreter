#!/bin/bash

# 对拍脚本 - 比较期望输出与实际输出
# 使用方法: ./compare.sh

# 设置颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 确保可执行文件存在
EXECUTABLE="./build/code"
TEST_DIR="./2394"

if [ ! -f "$EXECUTABLE" ]; then
    echo -e "${RED}错误: 可执行文件 $EXECUTABLE 不存在${NC}"
    echo "请先编译项目: cd build && make"
    exit 1
fi

if [ ! -d "$TEST_DIR" ]; then
    echo -e "${RED}错误: 测试目录 $TEST_DIR 不存在${NC}"
    exit 1
fi

# 计数器
total_tests=0
passed_tests=0
failed_tests=0

# 清空输出文件
> temp.out

echo -e "${BLUE}开始对拍测试...${NC}"
echo "========================================" | tee -a temp.out

# 遍历所有.in文件
for input_file in "$TEST_DIR"/*.in; do
    if [ ! -f "$input_file" ]; then
        continue
    fi
    
    # 提取测试编号
    test_name=$(basename "$input_file" .in)
    expected_file="$TEST_DIR/${test_name}.out"
    
    if [ ! -f "$expected_file" ]; then
        echo -e "${YELLOW}警告: 找不到对应的输出文件 $expected_file${NC}"
        continue
    fi
    
    total_tests=$((total_tests + 1))
    
    echo -e "${BLUE}测试 $test_name:${NC}" | tee -a temp.out
    
    # 运行程序获取实际输出，并去掉scm>提示符
    actual_output=$(timeout 10s "$EXECUTABLE" < "$input_file" 2>&1 | sed 's/^scm> //g' | sed '/^$/d')
    exit_code=$?
    
    if [ $exit_code -eq 124 ]; then
        echo -e "${RED}  超时错误${NC}"
        echo "测试 $test_name: 超时错误" >> temp.out
        failed_tests=$((failed_tests + 1))
        echo "----------------------------------------" | tee -a temp.out
        continue
    elif [ $exit_code -ne 0 ]; then
        echo -e "${RED}  运行时错误 (退出码: $exit_code)${NC}"
        echo "测试 $test_name: 运行时错误 (退出码: $exit_code)" >> temp.out
        failed_tests=$((failed_tests + 1))
        echo "----------------------------------------" | tee -a temp.out
        continue
    fi
    
    # 读取期望输出并处理行结束符差异（移除\r字符）
    expected_output=$(cat "$expected_file" | tr -d '\r')
    actual_output=$(echo "$actual_output" | tr -d '\r')
    
    # 将输出保存到临时文件进行逐行比较
    echo "$actual_output" > /tmp/actual_$test_name.tmp
    echo "$expected_output" > /tmp/expected_$test_name.tmp
    
    # 使用diff检查是否完全相同
    if diff -q /tmp/actual_$test_name.tmp /tmp/expected_$test_name.tmp > /dev/null; then
        echo -e "${GREEN}  通过${NC}"
        echo "测试 $test_name: 通过" >> temp.out
        passed_tests=$((passed_tests + 1))
    else
        echo -e "${RED}  失败${NC}"
        echo "测试 $test_name: 失败" >> temp.out
        failed_tests=$((failed_tests + 1))
        
        echo -e "${YELLOW}  输入内容:${NC}"
        cat "$input_file" | sed 's/^/    /'
        
        echo "输入内容:" >> temp.out
        cat "$input_file" | sed 's/^/  /' >> temp.out
        
        echo -e "${YELLOW}  差异详情:${NC}"
        echo "差异详情:" >> temp.out
        
        # 逐行比较并显示差异
        line_num=1
        while IFS= read -r expected_line && IFS= read -r actual_line <&3; do
            if [ "$expected_line" != "$actual_line" ]; then
                echo -e "${RED}    行 $line_num 不匹配:${NC}"
                echo -e "${BLUE}      期望输出:${NC} '$expected_line'"
                echo -e "${RED}      实际输出:${NC} '$actual_line'"
                
                echo "  行 $line_num 不匹配:" >> temp.out
                echo "    期望输出: '$expected_line'" >> temp.out
                echo "    实际输出: '$actual_line'" >> temp.out
            fi
            line_num=$((line_num + 1))
        done < /tmp/expected_$test_name.tmp 3< /tmp/actual_$test_name.tmp
        
        # 检查是否有额外的行
        expected_lines=$(wc -l < /tmp/expected_$test_name.tmp)
        actual_lines=$(wc -l < /tmp/actual_$test_name.tmp)
        
        if [ $expected_lines -gt $actual_lines ]; then
            echo -e "${RED}    实际输出缺少 $((expected_lines - actual_lines)) 行${NC}"
            echo "  实际输出缺少 $((expected_lines - actual_lines)) 行" >> temp.out
            tail -n +$((actual_lines + 1)) /tmp/expected_$test_name.tmp | while IFS= read -r line; do
                echo -e "${BLUE}      期望输出:${NC} '$line'"
                echo -e "${RED}      实际输出:${NC} (缺失)"
                echo "    期望输出: '$line'" >> temp.out
                echo "    实际输出: (缺失)" >> temp.out
                line_num=$((line_num + 1))
            done
        elif [ $actual_lines -gt $expected_lines ]; then
            echo -e "${RED}    实际输出多出 $((actual_lines - expected_lines)) 行${NC}"
            echo "  实际输出多出 $((actual_lines - expected_lines)) 行" >> temp.out
            tail -n +$((expected_lines + 1)) /tmp/actual_$test_name.tmp | while IFS= read -r line; do
                echo -e "${BLUE}      期望输出:${NC} (缺失)"
                echo -e "${RED}      实际输出:${NC} '$line'"
                echo "    期望输出: (缺失)" >> temp.out
                echo "    实际输出: '$line'" >> temp.out
                line_num=$((line_num + 1))
            done
        fi
    fi
    
    # 清理临时文件
    rm -f /tmp/actual_$test_name.tmp /tmp/expected_$test_name.tmp
    
    echo "----------------------------------------" | tee -a temp.out
done

# 输出总结
echo -e "${BLUE}测试完成!${NC}"
echo "========================================" | tee -a temp.out
echo -e "总测试数: $total_tests" | tee -a temp.out
echo -e "${GREEN}通过: $passed_tests${NC}"
echo -e "${RED}失败: $failed_tests${NC}"
echo "通过: $passed_tests" >> temp.out
echo "失败: $failed_tests" >> temp.out

if [ $failed_tests -eq 0 ]; then
    echo -e "${GREEN}所有测试都通过了! 🎉${NC}"
    echo "所有测试都通过了! 🎉" >> temp.out
    exit 0
else
    echo -e "${RED}有 $failed_tests 个测试失败 😞${NC}"
    echo "有 $failed_tests 个测试失败 😞" >> temp.out
    exit 1
fi
