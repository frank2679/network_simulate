% This file is the numerical analysis of my PS design of ax. 
% It is for estimate the system model. Every parameter could be seen. 
% The result will be synchorous to the function of system model. 
% 2016/5/26
% Frank
clear;
%% parameter (factors)
n = 10;% # of STAs
T = 10000;% period of TF 
la = 0.0002;% arrival rate x10^6

%% Constant (setting)
Tb = 50; % time of control frame
Tp = 500;% time of data frame
nch = 10; % # of channel
Cw = 32; % contension window size
Pow_TX = 1000;
Pow_RX = 600;
Pow_idle = 300;
Pow_doze = 150;

%for la = 0.00001:0.0001:0.001
%% expected value
nc = ceil((1-exp(-la*T))*n); % # of contending STAs % ! may < 1, because la is too small. 
Psc = ((Cw-1)/Cw)^(nc-1); % prob. of suc contending
%% compute pss &Ns
if nc < Cw
    loop_top = nc;
else
    loop_top = Cw;
end
Pss = 0;
temp_p(loop_top) = 0;
for i = 1:loop_top 
    temp_p(i) = prob(i,Cw,nc); % to reduce complexity
end
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
ns = Cw*nc*1/Cw*(1-1/Cw).^(nc-1);

R = nch/Ns;
Ps = Psc*Pss;
%% compute Ps & 
Pt = (1-exp(-la*T))/Ps;
UL = la*T/Ps;
delta_TX = 1/T * Pt * (Tb*(1-Ps) + Ps*(UL/R*Tp + Tb));
delta_RX = 1/T * Pt * (2*Tb*(1-Ps) + Ps*Tb*(UL + 2));
delta_idle = 1 - delta_TX - delta_RX;
delta_doze = 1 - delta_TX - delta_RX;
E_PS = delta_TX*Pow_TX + delta_RX*Pow_RX + delta_doze*Pow_doze;
E_NPS = delta_TX*Pow_TX + delta_RX*Pow_RX + delta_idle*Pow_idle;
eff = (E_NPS-E_PS)/E_NPS