clear;
%% Constants
nc = 10;
Cw = 32;
%% test prob. Epected value
% E[ns]
% ns_1 with prob. function
if nc < Cw
    loop_top = nc;
else
    loop_top = Cw; % the maximum Ns could take
end
temp_p(loop_top) = 0;
for i = 1:loop_top 
    temp_p(i) = prob(i,Cw,nc); % to reduce complexity
end
ns_1 = 0;
for i = 1:loop_top
    ns_1 = ns_1 + i*temp_p(i);
end
% ns_2
ns_2 = Cw*nc*1/Cw*(1-1/Cw).^(nc-1);