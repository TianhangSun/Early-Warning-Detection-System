% urlA = 'https://agent.electricimp.com/48H2Qgwd8We5';
urlB = 'https://agent.electricimp.com/6hmIZEP5UR5U';

% sensor0 = zeros(60, 1);
% sensor1 = zeros(60, 1);
% sensor2 = zeros(60, 1);
% sensor3 = zeros(60, 1);
% sensor4 = zeros(60, 1);
% sensor5 = zeros(60, 1);
% sensor6 = zeros(60, 1);
% sensor7 = zeros(60, 1);
% sensor8 = zeros(60, 1);
% sensor9 = zeros(60, 1);


while 1
    file = fopen('output.txt','w');
    i = 1;
    %for i = 1:60
    %strA = webread(urlA);
    strB = webread(urlB);
%     dataA = char(strA)';
%     sensor0(i) = str2num(dataA(23:27));
%     sensor1(i) = str2num(dataA(37:41));
%     sensor2(i) = str2num(dataA(51:55));
%     sensor3(i) = str2num(dataA(65:69));
%     sensor4(i) = str2num(dataA(79:83));
    
    dataB = char(strB)';
    sensor5(i) = str2num(dataB(9:13));
    sensor6(i) = str2num(dataB(24:28));
    sensor7(i) = str2num(dataB(39:43));
    sensor8(i) = str2num(dataB(54:58));
    sensor9(i) = str2num(dataB(69:73));
%     fprintf(file, '%s', dataA);
    fprintf(file, '%s , ', dataB);
    %i = i + 1;
    %pause(1);
 
    %end
    fclose(file);
    pause(1);
   
%     plot(sensor0);
%     hold on
%     plot(sensor1);
%     plot(sensor2);
%     plot(sensor3);
%     plot(sensor4);
%     plot(sensor5);
%     plot(sensor6);
%     plot(sensor7);
%     plot(sensor8);
%     plot(sensor9);
    
%    legend('Sensor0','Sensor1', 'Sensor2', 'Sensor3', 'Sensor4', 'Sensor5', 'Sensor6', 'Sensor7', 'Sensor8', 'Sensor9')
end

  




