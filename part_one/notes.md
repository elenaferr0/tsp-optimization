- considering acceleration wouldn't change the results as it would be like multiplying by a constant factor.

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


# Benchmark Results Comparison

## Before Optimization vs. After Optimization

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