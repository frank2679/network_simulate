% this function distribution function of randomly put m balls into n
% buckets. Use X denote num of buckets which contain one ball. 
% By invoking S function. 
% n: num of buckets
% m: num of balls
% P{X = k}
function [ p ] = prob( k, n, m )
%UNTITLED5 Summary of this function goes here
%   Detailed explanation goes here
if k <= n && k <=m
    p = 0;
    if k == 0
        for i = 1:m
            p = p + prob(i, n, m);
        end
        p = 1-p;
    else
        for i = k:n 
            %p = p+(-1)^(k+i) * nchoosek(i,k) * S(i,n,m);
            p = p+(-1)^(k+i) /factorial(i-k)/factorial(k) * S(i,n,m);
        end
    end
else
    p = 0; % for k > n or k > m, p = 0
end
end

