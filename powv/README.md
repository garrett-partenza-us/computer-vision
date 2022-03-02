Pow time: 0.00700092
Mul time: 0.00393009
Each operation was run one million times

The built in power function suffers from function call overhead, not being explicit code when compiled, and also being optimized to work with all real numbers. This causes the performance difference. Thus it is justified to use the pow function when not using integers and take advantage of the built in real-exponent optimization.