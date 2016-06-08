%% read data
input = fopen('output_1.dat', 'r');
formatSpec = '%f %f';
sizeA = [2 Inf];
A = fscanf(input, formatSpec, sizeA)
fclose(input);
plot(A(1,:), A(2,:));
hold on
input = fopen('output_10.dat', 'r');
formatSpec = '%f %f';
sizeA = [2 Inf];
A = fscanf(input, formatSpec, sizeA)
fclose(input);
plot(A(1,:), A(2,:));
legend('n=1', 'n=10');
xlabel('\lambda /s','FontSize', 16);
ylabel('Energy Saved', 'FontSize', 16);
ax.FontSize = 20;