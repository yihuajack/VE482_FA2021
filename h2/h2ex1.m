clear; clc;
n = 1:1:15;
p = [0.25 0.6 0.9];
hold on
for i = 1:3
    y = 1-p(i).^n;
    plot(n,y);
end
hold off
axis([1 15 0 1]);
xlabel('n',"FontName","Times New Roman","FontSize",18);
ylabel('CPU utilisation',"FontName","Times New Roman","FontSize",18);
title('CPU utilisation as a function of the number of processes n for different time fraction value p',"FontName","Times New Roman","FontSize",18);
legend('p=25%','p=60%','p=90%',"FontName","Times New Roman","FontSize",18);
