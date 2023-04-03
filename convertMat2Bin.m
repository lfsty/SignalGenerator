clc;clear;
filename =;
pathname =;

EEG = readbdfdata(filename, pathname);

EEGData = EEG.data;
event = EEG.event;
ch_num = EEG.nbchan + 1;
srate = EEG.srate;
% clear EEG;

% init marker
marker = zeros(1,size(EEGData,2));
for i=1:length(event)
    marker(event(i).latency) = str2num(event(i).type);
end

% write_data_total = [EEGData;marker];
% clear EEGData;
% clear event;

fid = fopen('data.bin','w');
fwrite(fid,ch_num,'int32');
fwrite(fid,srate,'int32');
fwrite(fid,size(EEGData,2),'int32');
for i = 1:size(EEGData,1)
    fwrite(fid,EEGData(i,:),'float');
end
fwrite(fid,marker,'int32');

fclose(fid);