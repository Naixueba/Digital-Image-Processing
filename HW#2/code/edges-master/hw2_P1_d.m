%% Tiger SE
F=[0.1944,0.2116,0.2357,0.2459,0.2424,0.2544,0.2472,0.2452,0.2387,0.2204,0.2439,0.2263];
Percision=[0.123,0.1397,0.1588,0.1668,0.1643,0.1748,0.1733,0.173,0.171,0.1618,0.1832,0.1766];
Recall=[0.4636,0.4356,0.4575,0.4682,0.4618,0.4671,0.431,0.4211,0.3954,0.3455,0.3647,0.315];
Threshold=[0.08,0.1,0.12,0.13,0.14,0.15,0.16,0.17,0.18,0.2,0.23,0.25];
figure(1);
plot(Threshold,F,'-o');
hold on;
plot(Threshold,Percision,'-xk');
hold on;
plot(Threshold,Recall,'-*r');
legend('F Measurement','Percision','Recall');
xlabel('Threshold'); grid on; title('Tiger SE Detection Plot');
%figure(2);
%plot(Recall, Percision);

%% Pig SE
F_Pig=[0.2231,0.2438,0.2501,0.2488,0.2529,0.2625,0.2664,0.2669,0.2664,0.2682,0.2638,0.2602,0.2514,0.2461];
Percision_Pig=[0.1511,0.1694,0.1736,0.1725,0.1758,0.1827,0.1861,0.1872,0.1879,0.1881,0.1826,0.1823,0.1799,0.1761];
Recall_Pig=[0.4259,0.4346,0.4475,0.4465,0.4504,0.4658,0.4685,0.4646,0.4576,0.467,0.4747,0.4539,0.417,0.4082];
Threshold_Pig=[0.08,0.1,0.12,0.13,0.14,0.15,0.16,0.17,0.18,0.19,0.2,0.22,0.24,0.25];
figure(2);
plot(Threshold_Pig,F_Pig,'-o');
hold on;
plot(Threshold_Pig,Percision_Pig,'-xk');
hold on;
plot(Threshold_Pig,Recall_Pig,'-*r');
legend('F Measurement','Percision','Recall');
xlabel('Threshold'); grid on; title('Pig SE Detection Plot');