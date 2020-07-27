clear;
close all;

%%
% define constants for 802.11a
B = 8224; % test frame size in bits
Oca = 75e-6; % channel access overhead
Op = 110e-6; % protocol overhead

%%
% function to calculate airtime
airtime = @(r,PER) (Oca+Op+B/r) * (1/(1-PER));

%%
% plot some results
r12 = [];
r48 = [];
r54 = [];
pers = [0.02 0.08 0.1 0.15 0.3 0.4 0.5];

for ii=1:size(pers,2)
    r12 = [r12 airtime(12e6, pers(ii))];
    r48 = [r48 airtime(48e6, pers(ii))];
    r54 = [r54 airtime(54e6, pers(ii))];
end

r12 = r12 * 1e3;
r48 = r48 * 1e3;
r54 = r54 * 1e3;

figure;
plot(pers, r12, '-^r');
hold on;
plot(pers, r48, '-vb');
hold on;
plot(pers, r54, '-sk');
grid on;
xlabel('PER');
ylabel('Airtime [ms]');
legend('R=12Mbps', 'R=48Mbps', 'R=54Mbps', 'Location', 'NorthWest');
title('Airtime link metric');

saveas(gcf,'airtime_lm','pdf');