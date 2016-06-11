% To run this file, change the function PS_ax_per output to eff_PS.
% This is file which invoke PS_ax_per(n,T,lambda). 
clear;
T = 10000;
lambda = 0.00002;
index = 1;
for n = 5:100
    eff_n(index) = PS_ax_per(n, T, lambda);
    index = index + 1;
end
n = 5:100;
plot(n, eff_n);
n_1 = 1;
n_2 = 10;
n_3 = 20;
n_4 = 40;
n_5 = 70;
n_6 = 100;
T_1 = 20000;
T_2 = 40000;
T_3 = 100000;
index = 1;
for lambda = 0.00001:0.00005:0.0003
    eff_la_1(index) = PS_ax_per(n_1, T, lambda);
    eff_la_2(index) = PS_ax_per(n_2, T, lambda);
    eff_la_3(index) = PS_ax_per(n_3, T, lambda);
    eff_la_4(index) = PS_ax_per(n_4, T, lambda);
    eff_la_5(index) = PS_ax_per(n_5, T, lambda);
    eff_la_6(index) = PS_ax_per(n_6, T, lambda);
    eff_T_1(index)  = PS_ax_per(n_2, T_1,lambda);
    eff_T_2(index)  = PS_ax_per(n_2, T_2,lambda);
    eff_T_3(index)  = PS_ax_per(n_2, T_3,lambda);
    index = index + 1;
end
lambda = 0.00001:0.00005:0.0003;
% figure 1
fig1 = plot(lambda, eff_la_1,'b--', lambda, eff_la_2,'b', lambda, eff_la_3,'r--', lambda, eff_la_4,'r', lambda, eff_la_5,'k--', lambda, eff_la_6,'k');
for i = 1:6
    fig1(i).LineWidth = 2;
end 
legend('n=1', 'n=10', 'n=20', 'n=40', 'n=70', 'n=100');
xlabel('\lambda /s','FontSize', 16);
ylabel('Energy Saved', 'FontSize', 16);
ax.FontSize = 20;
% figure 2
fig2 = plot(lambda, eff_T_1,'b-o', lambda, eff_T_2,'r-^',lambda, eff_T_3,'k-v');
for i = 1:3
    fig2(i).LineWidth = 2;
end 
legend('T=20ms','T=40ms','T=100ms');
fig2(1).MarkerFaceColor= 'b';
fig2(2).MarkerFaceColor= 'r';
fig2(3).MarkerFaceColor= 'k';
xlabel('\lambda /s','FontSize', 16);
ylabel('Energy Saved', 'FontSize', 16);