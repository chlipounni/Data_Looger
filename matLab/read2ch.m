clc
y = 780; x = 680;

fig = uifigure('Position',[100 50 x y], ...
    'Name','DATA-Logger');
ax = uiaxes('Parent',fig,...
    'Units','pixels',...
    'Position', [10 y-pos*32 x-20 300]);
errorFile = uilabel(fig, 'Position', [500 y-20*11 200 15], 'Text','taratata');
file = 'H:\1_3_DIV1.SMP';
fid = fopen(file,'r');

fseek(fid,4,"bof");
size = 1290;
mid1 = zeros(1,size);
mid2 = zeros(1,size);
x=1:size;

neg1 = zeros(1,size);
pos1 = zeros(1,size);
neg2 = zeros(1,size);
pos2 = zeros(1,size);
data = fread(fid,size*6,"uint16");
for z=0:1:size-1
    mid1(z+1)=data(2+6*z);
    neg1(z+1)=data(2+6*z)-data(1+6*z);
    pos1(z+1)=data(3+6*z)-data(2+6*z);
    mid2(z+1)=data(5+6*z);
    neg2(z+1)=data(5+6*z)-data(4+6*z);
    pos2(z+1)=data(6+6*z)-data(5+6*z);
end


mid2= mid2/2;
hold (ax,'on');
plot(ax,x,mid1,'Color','y');
 plot(ax,x, mid2);
 legend(ax,'cos(2x)','test2')
 hold (ax,'off');
