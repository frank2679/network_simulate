% To run this file, change the prob. function output to [ Ps, Psc, Pss ]. 
% See the num of STA affects the prob. of contending and RR
clear;
%% Parameter
T = 10000;
lambda = 0.0002;
%% test Ps, Prob. of suc transmission
for n = 1:100
    [Ps(n), Psc(n), Pss(n)] = PS_ax_per(n, T, lambda);
end
n = 1:100;
fig = plot(n, Ps, 'b-o', n, Psc, 'r-o', n, Pss, 'k-o');
legend('p_s', 'p_{sc}', 'p_{ss}');
xlabel('num of STA','FontSize', 16);
ylabel('probability', 'FontSize', 16);