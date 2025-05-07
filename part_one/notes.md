# Notes
- considering acceleration wouldn't change the results as it would be like multiplying by a constant factor.
- clear explanation of MTZ formulation: https://how-to.aimms.com/Articles/332/332-Miller-Tucker-Zemlin-formulation.html 
- implementation of MTZ: https://co-enzyme.fr/blog/traveling-salesman-problem-tsp-in-cplex-opl-with-miller-tucker-zemlin-mtz-formulation/
- checked with valgrind

TODO 1st
- other formulations which are not tsp?
- does it make sense to have gg and mtz?
- implement check to see how long it takes... 
- metrics/stats

TODO 2nd
- provide the average running times (compare several instances and the same instance multiple times if random components are present)
- Montecarlo simulation for heuristic
- tsp solution class?
- no need to do train/test/validation. mention if stoch component consider multiple runs
    - say that with different rules this this that is obtained therefore this rule is selected.
    - say that you're aware that this is not the most representative but for time constraints
    - be aware that validation has to be performed explain why and how
- i-race package
- deviation from best solution chart (by size) in percentage, how far away
- compare different methods (e.g. LS vs Tabu Search... ) averages, and obtained solutoin

# Benchmarking
### Variables/constraints creation
## Raw data
- no opt for vars/constraints:
100
[Variable creation] 0.007256s
[Constraints creation] 0.006306s
[Model setup] 0.013673s
500
[Variable creation] 0.067589s
[Constraints creation] 0.178245s
[Model setup] 0.247074s
1000
[Variable creation] 0.289972s
[Constraints creation] 0.762230s
[Model setup] 1.056627s
2000
[Variable creation] 1.171460s
[Constraints creation] 3.532657s
[Model setup] 4.721169s
5000
[Variable creation] 7.048614s
[Model setup] 35.029964s

- opt for vars+constraints:
Running benchmark for size 100
[Variable creation] Elapsed time: 0.007846s
[Constraint creation] Elapsed time: 0.004998s
[Model setup] Elapsed time: 0.013104s
Running benchmark for size 500
[Variable creation] Elapsed time: 0.068431s
[Constraint creation] Elapsed time: 0.064012s
[Model setup] Elapsed time: 0.134398s
Running benchmark for size 1000
[Variable creation] Elapsed time: 0.289487s
[Constraint creation] Elapsed time: 0.264862s
[Model setup] Elapsed time: 0.561145s
Running benchmark for size 2000
[Variable creation] Elapsed time: 1.217298s
[Constraint creation] Elapsed time: 1.128777s
[Model setup] Elapsed time: 2.376256s
Running benchmark for size 5000
[Variable creation] Elapsed time: 7.078701s
[Constraint creation] Elapsed time: 10.546984s
[Model setup] Elapsed time: 17.840437s

## Comparison

| Graph Size | Metric                | Before Optimization | After Optimization | Improvement (%) |
|------------|----------------------|---------------------|-------------------|-----------------|
| 100        | Variable creation    | 0.007256s          | 0.007846s        | -8.13%          |
|            | Constraint creation  | 0.006306s          | 0.004998s        | 20.74%          |
|            | Model setup          | 0.013673s          | 0.013104s        | 4.16%           |
| 500        | Variable creation    | 0.067589s          | 0.068431s        | -1.25%          |
|            | Constraint creation  | 0.178245s          | 0.064012s        | 64.09%          |
|            | Model setup          | 0.247074s          | 0.134398s        | 45.60%          |
| 1000       | Variable creation    | 0.289972s          | 0.289487s        | 0.17%           |
|            | Constraint creation  | 0.762230s          | 0.264862s        | 65.25%          |
|            | Model setup          | 1.056627s          | 0.561145s        | 46.89%          |
| 2000       | Variable creation    | 1.171460s          | 1.217298s        | -3.91%          |
|            | Constraint creation  | 3.532657s          | 1.128777s        | 68.05%          |
|            | Model setup          | 4.721169s          | 2.376256s        | 49.67%          |
| 5000       | Variable creation    | 7.048614s          | 7.078701s        | -0.43%          |
|            | Constraint creation  | N/A                | 10.546984s       | N/A             |
|            | Model setup          | 35.029964s         | 17.840437s       | 49.07%          |


## Max instance size solvable in time limit
Testing ./bin/main with various instance sizes and time constraints...
--------------------------------------------------
Testing time threshold: 0.1 seconds
Testing size 5... ✓
Testing size 10... ✓
Testing size 20... (timeout) ✗
Maximum size for 0.1s: 10

Testing time threshold: 0.5 seconds
Skipping size 5 (smaller than successful instance at lower threshold)
Skipping size 10 (smaller than successful instance at lower threshold)
Testing size 20... ✓
Testing size 50... (timeout) ✗
Maximum size for 0.5s: 20

Testing time threshold: 1 seconds
Skipping size 5 (smaller than successful instance at lower threshold)
Skipping size 10 (smaller than successful instance at lower threshold)
Skipping size 20 (smaller than successful instance at lower threshold)
Testing size 50... (timeout) ✗
Maximum size for 1s: 0

Testing time threshold: 5 seconds
Skipping size 5 (smaller than successful instance at lower threshold)
Skipping size 10 (smaller than successful instance at lower threshold)
Skipping size 20 (smaller than successful instance at lower threshold)
Testing size 50... (timeout) ✗
Maximum size for 5s: 0

Testing time threshold: 15 seconds
Skipping size 5 (smaller than successful instance at lower threshold)
Skipping size 10 (smaller than successful instance at lower threshold)
Skipping size 20 (smaller than successful instance at lower threshold)
Testing size 50... ✓
Testing size 100... ✓
Testing size 200... ✓
Testing size 500... ✓
Testing size 1000... ✓
Testing size 2000... (timeout) ✗
Maximum size for 15s: 1000

Testing time threshold: 30 seconds
Skipping size 5 (smaller than successful instance at lower threshold)
Skipping size 10 (smaller than successful instance at lower threshold)
Skipping size 20 (smaller than successful instance at lower threshold)
Skipping size 50 (smaller than successful instance at lower threshold)
Skipping size 100 (smaller than successful instance at lower threshold)
Skipping size 200 (smaller than successful instance at lower threshold)
Skipping size 500 (smaller than successful instance at lower threshold)
Skipping size 1000 (smaller than successful instance at lower threshold)
Testing size 2000... (timeout) ✗
Maximum size for 30s: 0

Testing time threshold: 60 seconds
Skipping size 5 (smaller than successful instance at lower threshold)
Skipping size 10 (smaller than successful instance at lower threshold)
Skipping size 20 (smaller than successful instance at lower threshold)
Skipping size 50 (smaller than successful instance at lower threshold)
Skipping size 100 (smaller than successful instance at lower threshold)
Skipping size 200 (smaller than successful instance at lower threshold)
Skipping size 500 (smaller than successful instance at lower threshold)
Skipping size 1000 (smaller than successful instance at lower threshold)
Testing size 2000... (timeout) ✗
Maximum size for 60s: 0

Testing time threshold: 120 seconds
Skipping size 5 (smaller than successful instance at lower threshold)
Skipping size 10 (smaller than successful instance at lower threshold)
Skipping size 20 (smaller than successful instance at lower threshold)
Skipping size 50 (smaller than successful instance at lower threshold)
Skipping size 100 (smaller than successful instance at lower threshold)
Skipping size 200 (smaller than successful instance at lower threshold)
Skipping size 500 (smaller than successful instance at lower threshold)
Skipping size 1000 (smaller than successful instance at lower threshold)
Testing size 2000... ✗
Maximum size for 120s: 0

Testing time threshold: 180 seconds
Skipping size 5 (smaller than successful instance at lower threshold)
Skipping size 10 (smaller than successful instance at lower threshold)
Skipping size 20 (smaller than successful instance at lower threshold)
Skipping size 50 (smaller than successful instance at lower threshold)
Skipping size 100 (smaller than successful instance at lower threshold)
Skipping size 200 (smaller than successful instance at lower threshold)
Skipping size 500 (smaller than successful instance at lower threshold)
Skipping size 1000 (smaller than successful instance at lower threshold)
Testing size 2000... ✗
Maximum size for 180s: 0

Testing time threshold: 240 seconds
Skipping size 5 (smaller than successful instance at lower threshold)
Skipping size 10 (smaller than successful instance at lower threshold)
Skipping size 20 (smaller than successful instance at lower threshold)
Skipping size 50 (smaller than successful instance at lower threshold)
Skipping size 100 (smaller than successful instance at lower threshold)
Skipping size 200 (smaller than successful instance at lower threshold)
Skipping size 500 (smaller than successful instance at lower threshold)
Skipping size 1000 (smaller than successful instance at lower threshold)
Testing size 2000... ✗
Maximum size for 240s: 0

Results saved to benchmark_20250507_223907.csv

Summary:
--------------------------------------------------
Time Threshold | Maximum Instance Size
--------------------------------------------------
0.1s           | 10
0.5s           | 20
1s             | 0
5s             | 0
15s            | 1000
30s            | 0
1min           | 0
2min           | 0
3min           | 0
4min           | 0


Time Threshold (s),Max Instance Size
0.1,10
0.5,20
1,0
5,0
15,1000
30,0
60,0
120,0
180,0
240,0
