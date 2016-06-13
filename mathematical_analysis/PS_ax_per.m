% T
function [ eff_PS ] = PS_ax_per( n, T, la )
%UNTITLED11 Summary of this function goes here
%   Detailed explanation goes here
Tb = 50; % us time of control frame
Tp = 500;% time of data frame
nch = 10; % # of channel
Cw = 32; % contension window size
Pow_TX = 1000;
Pow_RX = 600;
Pow_idle = 300;
Pow_doze = 150;
%% parameter (factors)
%n = 5;% # of STAs
%T = 10000;% us period of TF 
%la = 0.0005;% us arrival rate

%% expected value

nc = ceil((1-exp(-la*T))*n); % # of contending STAs
Psc = ((Cw-1)/Cw)^(nc-1); % prob. of suc contending
%% compute Pss &Ns
if nc < Cw
    loop_top = nc;
else
    loop_top = Cw;
end
temp_p(loop_top) = 0;
for i = 1:loop_top 
    temp_p(i) = prob(i,Cw,nc); % to reduce complexity
end
% compute Pss
Pss = 0;
for i = 1:loop_top
    if i <= nch
        Pss = Pss + temp_p(i);
    else
        Pss = Pss + nch/i*temp_p(i);
    end
end
% compute Ns
Ns = 0; 
for i = 1:loop_top
    if i < nch
        Ns = Ns + i*temp_p(i);
    else
        Ns = Ns + nch*temp_p(i);
    end
end
%% two ways to compute ns to validate the prob. function.  
ns_2 = Cw*nc*1/Cw*(1-1/Cw).^(nc-1);
ns_1 = 0;
for i = 1:loop_top
    ns_1 = ns_1 + i*temp_p(i);
end
%% Others
R = nch/Ns;
Ps = Psc*Pss;
Pt = (1-exp(-la*T))/(1-(1-Ps)*exp(-la*T));
UL = la*T*(2-Ps);
delta_TX = 1/T * Pt * (Tb*(1-Ps) + Ps*(UL/R*Tp + Tb));
delta_RX = 1/T * Pt * (2*Tb*(1-Ps) + Ps*Tb*(UL + 2));
delta_idle = 1 - delta_TX - delta_RX;
delta_doze = 1 - delta_TX - delta_RX;
E_PS = delta_TX*Pow_TX + delta_RX*Pow_RX + delta_doze*Pow_doze;
E_NPS = delta_TX*Pow_TX + delta_RX*Pow_RX + delta_idle*Pow_idle;
eff_PS = (E_NPS-E_PS)/E_NPS;
end

