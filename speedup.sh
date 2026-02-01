#!/bin/bash

# Usage: ./analyze_performance.sh <binary_name>
BINARY=$1
PERF_BIN="/usr/lib/linux-tools/6.8.0-94-generic/perf"

if [ -z "$BINARY" ]; then
    echo "Usage: $0 <binary_name>"
    exit 1
fi

OUTPUT_FILE="performance_metrics.csv"
echo "Threads,Time_Elapsed,User_Time,Sys_Time,CPU_Utilization" > "$OUTPUT_FILE"

THREAD_COUNTS=(1 2 4 5 6 10 11 12)

echo "Starting WSL-compatible performance analysis for $BINARY..."

for T in "${THREAD_COUNTS[@]}"; do
    echo "---------------------------------------"
    echo "Running with $T threads..."

    export OMP_NUM_THREADS=$T

    # Run once normally (optional)
    ./"$BINARY" > /dev/null

    # Run with perf (software counters only)
    $PERF_BIN stat \
        -o perf_tmp.txt \
        -e task-clock,context-switches,cpu-migrations,page-faults \
        ./"$BINARY" > /dev/null 2>&1

    TIME=$(grep "seconds time elapsed" perf_tmp.txt | awk '{print $1}')
    USER=$(grep "seconds user" perf_tmp.txt | awk '{print $1}')
    SYS=$(grep "seconds sys" perf_tmp.txt | awk '{print $1}')
    UTIL=$(grep "CPUs utilized" perf_tmp.txt | awk '{print $4}')

    echo "$T,$TIME,$USER,$SYS,$UTIL" >> "$OUTPUT_FILE"
done







rm -f perf_tmp.txt

echo "---------------------------------------"
echo "Analysis complete. Data saved to $OUTPUT_FILE"
