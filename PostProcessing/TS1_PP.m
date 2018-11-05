function [LP1,LP2,LP3,LP4,data5,data6,data7,data8,data9,data10] = TS1_PP(filename)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

data = csvread(filename);
LP1 = data(:,1);
LP2 = data(:,2);
LP3 = data(:,3);
LP4 = data(:,4);
data5 = data(:,5);
data6 = data(:,6);
data7 = data(:,7);
data8 = data(:,8);
data9 = data(:,9);
data10 = data(:,10);

%% Linear Potentiometer data comes in from a 12 bit ADC
% y=mx+b
LP_cal_1_m = 1; % Calibration for LP1 slope
LP_cal_2_m = 1; % Calibration for LP2 slope
LP_cal_3_m = 1; % Calibration for LP3 slope
LP_cal_4_m = 1; % Calibration for LP4 slope
LP_cal_1_b = 0; % Calibration for LP1 intercept
LP_cal_2_b = 0; % Calibration for LP2 intercept
LP_cal_3_b = 0; % Calibration for LP3 intercept
LP_cal_4_b = 0; % Calibration for LP4 intercept

for i=1:length(LP1)
    LP1(i) = LP1(i)*LP_cal_1_m + LP_cal_1_b;
    LP2(i) = LP1(i)*LP_cal_2_m + LP_cal_2_b;
    LP3(i) = LP1(i)*LP_cal_3_m + LP_cal_3_b;
    LP4(i) = LP1(i)*LP_cal_4_m + LP_cal_4_b;
end



end