clear ; close all; clc

data = load("ratios.txt");
y = data(:, 1);

m = length(y);


X = 1:size(y);
X = X';
X = X .*10000;
x = [ones(m,1), 1./X, 1./log(X)];


theta = zeros(3,1);

theta = (inv(x'*x))*x'*y




plot(X,y,"rx","Markersize",0.5);
xlabel("Nombre");
ylabel("Premiers en %");


hold on; % keep previous plot visible
plot(X, x*theta, '-k', "Markersize",100)
legend('Points', 'Tendance (r�gression lin�aire)')
hold off % don't overlay any more plots on this figure

taux_mille = [1, 1/10000, 1/log(10000)] * theta

taux_limite_cinq =  [1, 1/500000000, 1/log(500000000)] * theta

taux_limite_10pow9 =  [1, 1/1000000000, 1/log(1000000000)] * theta

taux_limite_2M =  [1, 1/2000000000, 1/log(2000000000)] * theta

taux_limite_10pow10 =  [1, 1/10000000000, 1/log(10000000000)] * theta

taux_limite_10pow11 =  [1, 1/100000000000, 1/log(100000000000)] * theta

taux_limite_10pow12 =  [1, 1/1000000000000, 1/log(1000000000000)] * theta

taux_limite_10pow13 =  [1, 1/10000000000000, 1/log(10000000000000)] * theta

diff = taux_mille - taux_limite_cinq


pause;