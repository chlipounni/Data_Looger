file = 'H:\1_3_DIV1.SMP';
fid = fopen(file)
A = fread(fid, 4, 'uint8')
A = fread(fid, inf, 'uint16');
plot(A)
B = reshape(A,2,size(A,1)/2)';

B1 = A((1:6:size(A,1)-1));
B2 = A((5:6:size(A,1)));

plot(B2)
%plot(abs(diff(B)))
