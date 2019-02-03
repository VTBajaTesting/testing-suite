function [time,LP1,LP2,LP3,LP4,accelOne_x,accelOne_y,accelOne_z,accelTwo_x,accelTwo_y,accelTwo_z] = TS1_PP(filename)
%TS1_PP is Testing Suite 1 - Post processing for VT Baja SAE
%   

data = csvread(filename);
time = data(:,1)/1000;
LP1 = data(:,2);
LP2 = data(:,3);
LP3 = data(:,4);
LP4 = data(:,5);
accelOne_x = data(:,6);
accelOne_y = data(:,7);
accelOne_z = data(:,8);
accelTwo_x = data(:,9);
accelTwo_y = data(:,10);
accelTwo_z = data(:,11);

%% Linear Potentiometer data comes in from a 12 bit ADC
% y=mx+b    -->>    x=(y-b)/m
% According to the calibration curves that have been generated, all 4 LPs
% can be calibrated with the same curve with an R^2=1. Please see the
% following server location for more information:
% B:\Baja2019\Testing\Calibration Data\LinearPotentiometer(250mm Stroke) -
% Calibration Data.xlsx

LP_cal_1_m = -415.83; % Calibration for LP1 slope
LP_cal_2_m = -415.83; % Calibration for LP2 slope
LP_cal_3_m = -415.83; % Calibration for LP3 slope
LP_cal_4_m = -415.83; % Calibration for LP4 slope
LP_cal_1_b = 4094.8; % Calibration for LP1 intercept
LP_cal_2_b = 4094.8; % Calibration for LP2 intercept
LP_cal_3_b = 4094.8; % Calibration for LP3 intercept
LP_cal_4_b = 4094.8; % Calibration for LP4 intercept

for i=1:length(LP1)
    if(LP1(i)>4095)
        LP1(i)=4095;
    end
    if(LP1(i)<0)
        LP1(i)=0;
    end
    if(LP2(i)>4095)
        LP2(i)=4095;
    end
    if(LP2(i)<0)
        LP2(i)=0;
    end
    if(LP3(i)>4095)
        LP3(i)=4095;
    end
    if(LP3(i)<0)
        LP3(i)=0;
    end
    if(LP4(i)>4095)
        LP4(i)=4095;
    end
    if(LP4(i)<0)
        LP4(i)=0;
    end
end

for i=1:length(LP1)
    LP1(i) = (LP1(i) - LP_cal_1_b) / LP_cal_1_m;
    LP2(i) = (LP2(i) - LP_cal_2_b) / LP_cal_2_m;
    LP3(i) = (LP3(i) - LP_cal_3_b) / LP_cal_3_m;
    LP4(i) = (LP4(i) - LP_cal_4_b) / LP_cal_4_m;
end

figure('name','Accelerometer Data');
plot(time,LP1,time,LP2,time,LP3,time,LP4);
legend("LP1","LP2","LP3","LP4");
xlabel("Time [s]");
ylabel("Shock Extension [in]");
grid on

%% Accelerometer data comes in 12 bit signed hex
%range - input the range (in g's) the sensor was in
range = 8;
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