% this function is invoked by prob. function. 
% n: num of buckets
% m: num of balls
function [ Sk ] = S( j, n, m )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
if j <= m
    %Sk = nchoosek(n,j);
    Sk = factorial(n)/factorial(n-j);
    for i = 0:(j-1)
        Sk = Sk*(m-i)/(n-i)*(1-1/(n-i))^(m-i-1);
    end
else
    Sk = 0;
end
end

