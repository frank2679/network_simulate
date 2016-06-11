clear;
lambda = 0.0002;
T = 10000;
n = 50;
Cw = 32;
Pss = 0.871293921001801;

syms nc ps;
[solnc, solps] = solve([nc == (1-exp(-lambda*T))*n/(1-(1-ps)*exp(-lambda*T)), ps == ((Cw-1)/Cw)^(nc-1)*Pss],[nc, ps])