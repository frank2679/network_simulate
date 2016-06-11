close;
clear;

fig1=open('test_per_compare.fig');
fig2=open('simulate_n.fig');

ax1=get(fig1,'Children');
ax2=get(fig2,'Children');

for i = 1 : numel(ax2) 
    ax2Children = get(ax2(i),'Children');
    % ax1Children= get(ax1(i),'Children');
    hold on;
    % first=copyobj(ax1Children, ax1(i));
    second=copyobj(ax2Children, ax1(i));
end

% set(first,'marker','o');
set(second,'marker','v');
%legend(first, {'first'}, 'Location','NorthWest', 'Color','g');
%legend(second, {'second'}, 'Location','NorthWest', 'Color','b');