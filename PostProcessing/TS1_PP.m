function [LP1,LP2,LP3,LP4,accelOne_x,accelOne_y,accelOne_z,accelTwo_x,accelTwo_y,accelTwo_z] = TS1_PP(filename)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

data = csvread(filename);
LP1 = data(:,1);
LP2 = data(:,2);
LP3 = data(:,3);
LP4 = data(:,4);
accelOne_x = data(:,5);
accelOne_y = data(:,6);
accelOne_z = data(:,7);
accelTwo_x = data(:,8);
accelTwo_y = data(:,9);
accelTwo_z = data(:,10);

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
%% Accelerometer data comes in 12 bit signed hex
%range - input the range (in g's) the sensor was in
range = 8
for i=1:length(accelOne_x)
 accelOne_x(i) = hex2dec(accelOne_x(i));
if( accelOne_x(i) <= 2^11)
    accelOne_x(i) = -(2^12 - accelOne_x(i));
end
accelOne_x(i) = accelOne_x(i) /(2048/range);

 accelOne_y(i) = hex2dec(accelOne_y(i));
if( accelOne_y(i) <= 2^11)
    accelOne_y(i) = -(2^12 - accelOne_y(i));
end
accelOne_y(i) = accelOne_y(i) /(2048/range);

 accelOne_z(i) = hex2dec(accelOne_z(i));
if( accelOne_z(i) <= 2^11)
    accelOne_z(i) = -(2^12 - accelOne_z(i));
end
accelOne_z(i) = accelOne_z(i) /(2048/range);


 accelTwo_x(i) = hex2dec(accelTwo_x(i));
if( accelTwo_x(i) <= 2^11)
    accelTwo_x(i) = -(2^12 - accelTwo_x(i));
end
accelTwo_x(i) = accelTwo_x(i) /(2048/range);

 accelTwo_y(i) = hex2dec(accelTwo_y(i));
if( accelTwo_y(i) <= 2^11)
    accelTwo_y(i) = -(2^12 - accelTwo_y(i));
end
accelTwo_y(i) = accelTwo_y(i) /(2048/range);

 accelTwo_z(i) = hex2dec(accelTwo_z(i));
if( accelTwo_z(i) <= 2^11)
    accelTwo_z(i) = -(2^12 - accelTwo_z(i));
end
accelTwo_z(i) = accelTwo_z(i) /(2048/range);
end