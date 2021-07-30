file = 'H:\1_0_DIV0.SMP';
fid = fopen(file)
A = fread(fid, inf, 'uint16');
plot(A)
B = reshape(A,2,size(A,1)/2)';

B1 = A((1:2:size(A,1)-1));
B2 = A((2:2:size(A,1)));

%plot(B1)
%plot(abs(diff(B)))
