clear ; close all; clc

data = load("vitesse.txt");
y = data(:, 1);
y = [0;y];
X = 1:size(y);
X = X'

figure;

plot(X,y,"-m","Markersize",10);
xlabel("X*10k premiers");
ylabel("Average speed");

pause;

theta = zeros(2,1);
X = [ones(size(y)) X];

theta = (inv(X'*X))*X'*y;

hold on;
plot(X(:,2), X*theta, "-k", "Markersize", 1000);
legend("Vitesse", "Evolution");
hold off;