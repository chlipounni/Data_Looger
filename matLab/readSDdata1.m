clc
file = 'E:\0_1_DIV0.SMP';
fid = fopen(file);
A = fread(fid, inf, 'uint16');
%plot(A)
%B = reshape(A,2,size(A,1)/2)';

%B1 = A((1:6:size(A,1)-1));
%B2 = A((5:6:size(A,1)))
B1ch1 = A((2:2:10000000));
%B2ch1 = A((0:2:size(A,1)));
%plot(B2)
plot(abs(diff(B1ch1)))
