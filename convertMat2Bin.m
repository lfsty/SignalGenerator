clc;clear;
filename =;
pathname =;
savefilename = 'data.bin';
EEG = readbdfdata(filename, pathname);

EEGData = EEG.data;
event = EEG.event;
ch_num = EEG.nbchan + 1;
srate = EEG.srate;
ch_name = {EEG.chanlocs.labels};
ch_name{1,end+1} = 'Marker';
% clear EEG;

% init marker
marker = zeros(1,size(EEGData,2));
for i=1:length(event)
    marker(event(i).latency) = str2num(event(i).type);
end

fid = fopen(savefilename,'w','l','UTF-8');

fwrite(fid,ch_num,'uint32');
fwrite(fid,srate,'uint32');
fwrite(fid,size(EEGData,2),'uint32');
for i=1:ch_num
    tmp_ch_name = ch_name{1,i};
    fwrite(fid,length(tmp_ch_name),'uint8');
    fwrite(fid,tmp_ch_name,'char');
end

for i = 1:size(EEGData,1)
    fwrite(fid,EEGData(i,:),'float');
end
fwrite(fid,marker,'float');

fclose(fid);