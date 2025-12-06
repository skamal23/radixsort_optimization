This is my assignment details:

CSEE W4824 – Computer Architecture
Homework Assignment 4
Assigned: 11.10.2025, Due: 12.08.2025, 11:59 pm ET
Collaboration Guidelines: Students may work in groups of up to 4. Please follow all
Columbia academic guidelines with respect to academic integrity. In addition, keep a log of
what days/times you worked on the project, which team members were present, and what
specific work item was completed. Submit this log along with your homework. Additionally,
ensure that any code or data involved with this homework is also submitted.
Submission Guidelines: One submission per group of 4, submitted through Course-
Works. Please attach any generated datasets or written code as part of your submission.
No extensions can be accommodated on this assignment due to the turnaround for grading
at the end of the semester.
Objective
Your task is to implement an efficient, stable sorting algorithm for large in-memory datasets
(typically in gigabytes) consisting of either 32 bit integers or floating-point numbers. The
primary goal is to optimize for cost-efficiency, measured as the total cost of sorting per
gigabyte.
You are expected to:
• Develop an efficient and stable sorting algorithm that operates on data pinned in
memory (DRAM).
• Tailor your implementation to leverage specific features of the processor and memory
system available on either a chosen CloudLab machine or on hardware you have access
to. You may also petition Professor Sethumadhavan for Google Cloud Credits if
interested. Please include your CloudLab/cloud instance type in your report if you
choose this option.
• Optimize the cost of sorting per GB by minimizing runtime, selecting appropriate
hardware, and using architectural insights.
1
Assignment Details
Algorithm Selection and Data Structure Design
• Choose a stable sorting algorithm that meets the in-memory constraints of large data
volumes. You may consider algorithms like Merge Sort, Timsort, Radix Sort, or other
appropriate stable sorting methods.
• Select data structures that minimize memory footprint and optimize for cache locality,
given the processors cache hierarchy and DRAM characteristics.
Questions to consider:
• What are the tradeoffs of the algorithm(s) you selected in terms of speed, memory
usage, and stability?
• How do the chosen data structures support efficient data access patterns for large
datasets?
Processor and Memory System Optimization
Leverage architectural features of the processor and memory system to improve sorting
performance. This may include:
• Optimizing for memory system configuration, cache usage, minimizing memory la-
tency
• Taking advantage of parallelism provided by multi-core processors
• Using special purpose instructions, etc.
• Utilizing any programming language, libraries, or even hand-optimized assembly code
• Applying machine learning techniques to select the optimal processor instance for
sorting based on data characteristics
• Reimplementing algorithms from research papers for sorting
Questions to Consider:
• What architectural features are you exploiting to optimize performance? How do they
enhance sorting speed or reduce memory usage?
• If using a CloudLab machine, what is your rationale for choosing a particular instance
type, considering both performance and cost?
2
Performance Measurement and Cost Calculation
• Measure the runtime and memory usage of your sorting implementation
• Based on these measurements, calculate the cost of sorting per GB using the pricing
structure of your chosen hardware or CloudLab machine. For example, if running
your machine might be estimated to cost $25/month, use this rate to calculate the
cost per GB based on your sorting time.
Questions to Consider:
• How did you determine and calculate the total sorting cost per GB?
• What performance metrics did you gather (e.g., execution time, CPU cycles, memory
usage), and how did they influence your final cost calculation?
Testing and Validation
• Test your sorting algorithm on sample datasets of varying sizes and characteristics to
ensure scalability and correctness
• Document any challenges or limitations encountered during testing and how you ad-
dressed them
Analysis and Report
Write a report explaining:
• Your choice of algorithms and data structures
• The optimizations applied based on processor and memory characteristics
• A breakdown of performance measurements and final cost per GB calculations.
• A post-mortem analysis that reflects on the effectiveness of your implementation,
highlights any potential areas for future improvement, and discusses any observed
limitations.
Questions to consider:
• Why do you believe your algorithm and data structures were well-suited for this task?
• What insights did you gain from the post-implementation analysis regarding your
algorithm’s efficiency and cost-effectiveness?
3
Submission Requirements
• Code: Submit the source code (named sorting.c in your Courseworks submission) of
your implementation and test cases. Please document your code and include comments
for readability.
• Report: Provide a detailed report (3-5 pages) covering the points outlined above.
Your report should be written in the format of a technical report and include at least
the following sections: introduction, methodology, results, discussion, and conclusion.
DO NOT exceed 5 pages.
• Cost Analysis: your measurements
Include a clear calculation of the cost per GB for sorting based on
Evaluation Criteria
• Correctness and Stability (5 pts): The implementation must produce a stable
sort and correctly handle large datasets.
• Cost-Efficiency (10 pts): Minimization of the sorting cost per GB will be a primary
metric for evaluation. Include graphs and data to show that your implementation is
cost-effective for varying input sizes. Include your calculation of the cost per GB with
and without optimizations.
• Optimization and Innovation (10 pts): Effective use of processor/memory ar-
chitecture and any innovative techniques for performance improvement will be highly
valued. Please answer the questions provided in the assignment in your report and ex-
plain any architectural design choices you made in your implementation. Your report
muts include a detailed discussion of at least three optimization strategies. Include
data and graphs to support your design choices.
• Report Quality (5 pts): Clarity, depth of analysis, and thoroughness in the final
report



This is my plan, which I implemented in baseline.c and optimized.c:
Project Master Plan: High-Performance In-Memory SortingTarget Architecture: AMD EPYC "Rome" (Zen 2)Objective: Minimize Cost-per-GB via Architecture-Aware Software Design.Phase 1: Strategic Architecture (The Hardware & Cost Defense)Before writing code, we must mathematically justify our hardware choice. The assignment asks to optimize Cost Efficiency.1.1 The Hardware Selection: "Right-Sizing" for Memory BandwidthWe select the CloudLab c6525-25g node over the c6525-100g.Node A: c6525-25gCPU: AMD EPYC 7302P (16 Cores / 32 Threads, 3.0 GHz Base).Architecture: 4 Core Complexes (CCXs) $\times$ 4 Cores.Memory: 128GB DDR4-3200 (8 Channels).Node B: c6525-100gCPU: AMD EPYC 7402P (24 Cores / 48 Threads, 2.8 GHz Base).Architecture: 6 CCXs $\times$ 4 Cores.Memory: 128GB DDR4-3200 (8 Channels).The Engineering Defense:The Memory Wall: Both CPUs utilize the same 8-channel DDR4 controller. Theoretical bandwidth for both is $\approx 204.8$ GB/s. Since Radix Sort is memory-bound (scanning and scattering data), adding cores beyond the saturation point yields diminishing returns.Clock Speed: The 16-core 7302P has a higher base clock (3.0 GHz) than the 24-core 7402P (2.8 GHz). For the serial portions of the code (prefix sums), the 16-core chip is faster per-thread.1.2 The Economic Model (Pricing Details)To calculate cost efficiency, we map these CloudLab nodes to their closest AWS EC2 equivalents to generate a "Shadow Price."Equivalent Family: AWS m6a (AMD EPYC Gen 3, comparable cost structure).Pricing Structure (US East N. Virginia):16-Core Node (m6a.4xlarge equivalent): $\approx \$1.38 / \text{hour}$.24-Core Node (m6a.8xlarge equivalent): $\approx \$2.07 / \text{hour}$ (Cost scales linearly with core count).The "Break-Even" Calculation:The 24-core node is 50% more expensive ($\frac{2.07}{1.38} = 1.5$).To justify using the 24-core node, our sort would need to run 50% faster.Hypothesis: Because we are limited by the 204 GB/s memory bus, moving from 16 to 24 cores will not yield a 50% speedup. It will likely yield <15% speedup.Conclusion: The 16-core node is the mathematically superior choice for Cost/GB.Phase 2: The Algorithmic Core (Pure C)We strictly separate the Control Group from the Target Implementation. Both are in C.2.1 The Control Group: radix_naive.cThis serves as the baseline to demonstrate "Algorithm without Architecture."Implementation: Single-threaded LSD Radix Sort.Memory: Standard malloc (4KB pages).Structure:Global Histogram (count[256]).Full-array passes (No cache blocking).Scalar operations (No SIMD).Purpose: This represents $O(N)$ efficiency before hardware optimization. It will suffer from TLB thrashing and Cache Line conflicts.2.2 The Target: radix_optimized.cAlgorithm: Parallel LSD Radix Sort (Base 256).Why LSD? It is iteratively stable, removing the need for complex recursive stack management required by MSB Radix.Why Base 256? Fits the 8-bit byte boundary, simplifying bitwise masking (val & 0xFF), and creates a small histogram (1KB) that fits in L1 Cache.2.3 The "Float" Solution (Bijection Transform)Constraint: Radix sort relies on bitwise operations. IEEE-754 floats do not sort correctly if treated as integers (Negative numbers have MSB=1, making them look "large").The Transform:Cast float bits to uint32_t.Mask Gen: mask = (-(int32_t)(u >> 31)) | 0x80000000Flip: u ^= maskResult: Positive floats have sign bit flipped ($0 \to 1$). Negative floats have all bits flipped. This creates a monotonic integer mapping.Cost: This is an $O(N)$ pass, but it is vectorizable.Phase 3: The Optimization Stack (The "How")We implement these layers cumulatively.Layer 1: Memory Hierarchy (L2 Cache Blocking)The Hardware Reality: The Zen 2 L2 cache is 512 KB.The Problem: Scanning a 1GB array evicts the start of the array from cache before the pass is finished.The Fix: Process data in 256 KB Chunks.Load 256KB of data into L2.Compute Histograms.Scatter to destination (write-combining buffer).Result: The read-phase latency drops from DRAM speeds (~100ns) to L2 speeds (~12 cycles).Layer 2: Parallelism & False Sharing (Thread Privatization)The Problem: If 16 threads increment a global count[256] array, cache lines will "ping-pong" between cores (Invalidate-Read-Modify-Write cycle), destroying performance.The Fix:Private Histograms: Each thread has its own local_count[256] in stack memory (L1 Cache).Barrier: pragma omp barrierReduction: Merge local counts into a global offset table using a Prefix Sum.Layer 3: Topology Awareness (CCX Pinning)The Hardware Reality: The EPYC 7302P has 4 CCXs. Cores 0-3 share L3 Slice A. Cores 4-7 share L3 Slice B.The Risk: If the OS migrates Thread 0 from Core 0 to Core 4, the thread loses its hot L3 cache lines.The Fix: Explicit Affinity.pthread_setaffinity_np / OpenMP OMP_PLACES.Pin Thread $T$ to Core $T$.Ensure threads operating on adjacent data blocks reside on the same CCX to maximize "L3 Constructive Interference."Layer 4: Virtual Memory (Hugepages)The Math: 10GB Data / 4KB Page = 2,621,440 Pages.The Bottleneck: The Translation Lookaside Buffer (TLB) cannot hold 2.6M entries. Every memory access triggers a Page Table Walk (expensive).The Fix: Transparent Hugepages (THP).10GB / 2MB Page = 5,120 Pages.This fits entirely within the L2 TLB of the EPYC processor.Phase 4: "Reach" Optimizations (Deep Cuts)These optimizations target the specific microarchitecture of the memory controller.4.1 Non-Temporal Stores (Streaming Stores)The Bottleneck: "Write Allocation." When the CPU writes to the output array, it first reads the destination cache line from RAM to L3/L2, modifies it, and eventually writes it back. This wastes 50% of memory bandwidth reading "garbage" data we are about to overwrite.The Fix: _mm256_stream_si256 (AVX2 Intrinsic).Effect: Bypass the cache hierarchy. Write directly to DRAM (Write Combining Buffer).Implementation Note: Requires memory to be 32-byte aligned (posix_memalign).4.2 Software PrefetchingThe Bottleneck: The "Scatter" phase of Radix Sort is random access. The hardware prefetcher cannot predict where output[ count[key]++ ] will land.The Fix: __builtin_prefetch.Logic: When processing index i, issue a prefetch for the bucket destination of index i+16. This overlaps the memory fetch latency with the current computation.4.3 Loop Unrolling (ILP)The Logic: The inner loop of Radix sort is tight.idx = (val >> shift) & 0xFF;dst[ count[idx]++ ] = val;Optimization: Unroll 4x manually to break dependency chains and allow the CPU's Out-of-Order execution engine to issue multiple loads/stores per cycle.Phase 5: Experimental Suite (The Dimensions)We run these experiments to generate the data for the report.Experiment A: The Memory Wall (Scalability)Config: radix_optimized on 10GB Random Data.Variable: Threads (1, 2, 4, 8, 12, 16).Hypothesis: Speedup will be linear up to ~12 threads, then curve flat.Significance: This curve proves that the Memory Bandwidth is the bottleneck, validating our choice to reject the 24-core node.Experiment B: The "Sweet Spot" (Block Size)Config: radix_optimized (16 threads).Variable: Block Size (16KB, 32KB, 128KB, 256KB, 512KB, 2MB).Hypothesis: 256KB will perform best (fits in L2). 512KB will degrade (L2 conflicts). 2MB will degrade further (L3 latency).Significance: Proves knowledge of the Zen 2 cache hierarchy.Experiment C: Distribution RobustnessConfig: radix_optimized vs naive_radix.Variable: Input Data.Uniform Random.Sorted (Ascending).Reverse Sorted.Gaussian (Normal Distribution) - Stress Test for bucket contention.Hypothesis: Radix sort runtime will be invariant (flat line).Significance: Demonstrates stability compared to Quicksort (which degenerates on sorted data).Experiment D: The Final Metric (Cost Efficiency)Formula:$$Cost/GB = \frac{\text{Runtime(s)} \times \$1.3824}{3600 \times \text{Size(GB)}}$$Comparison: Plot this metric for:Naive Implementation.Optimized Implementation on 16-Core Node.(Projected) Optimized Implementation on 24-Core Node (using 1.1x speedup assumption vs 1.5x cost).Goal: Prove the 16-Core Optimized version provides the lowest global minimum Cost/GB.Implementation Checklist (The "Nitty Gritty")Alignment: Ensure all arrays are 32-byte aligned (posix_memalign).Verification:verify_sorted(arr, n)verify_stability(arr, n) (Use tagged structs to force duplicates and check index order).Timing: Use clock_gettime(CLOCK_MONOTONIC) for nanosecond precision, excluding data generation time.Compilation: gcc -O3 -march=znver2 -fopenmp -mavx2 sorting.c -o sorting.
