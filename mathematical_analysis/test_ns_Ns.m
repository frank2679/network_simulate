% To run this file, change the prob. function output to [ ns_1, ns_2, Ns]. 
% See the num of STA affects the prob. of contending and RR
clear;
%% Parameter
T = 10000;
lambda = 0.0002;
%% test Ps, Prob. of suc transmission
for n = 1:100
    [ns_1(n), ns_2(n), Ns(n)] = PS_ax_per(n, T, lambda);
end
n = 1:100;
fig = plot(n, ns_1, 'b-o', n, ns_2, 'r-o', n, Ns, 'k-o');
fig(1).LineWidth = 2;
legend('n_{s1}', 'n_{s2}', 'N_s');
xlabel('num of STA','FontSize', 16);
%ylabel('expected value', 'FontSize', 16);