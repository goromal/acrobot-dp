function u = read_u_ab_log(logname)

NUM_TH1 = 18;
NUM_TH2 = 36;
NUM_THD1 = 144;
NUM_THD2 = 324;
NUM_DOUBLES = NUM_TH1 * NUM_TH2 * NUM_THD1 * NUM_THD2;

data_log = fopen(logname,'r');
data_lin = fread(data_log, 'double');
data_lin = reshape(data_lin, 1, []);
fclose(data_log);

u = zeros(NUM_TH1,NUM_TH2,NUM_THD1,NUM_THD2);

i = 1;
j = 1;
k = 1;
l = 1;

for idx = 1:1:NUM_DOUBLES
   
    u(i,j,k,l) = data_lin(idx);
    
    l = l + 1;
    if l > NUM_THD2
        l = 1;
        k = k + 1;
    end
    if k > NUM_THD1
        k = 1;
        j = j + 1;
    end
    if j > NUM_TH2
        j = 1;
        i = i + 1;
    end
    
end

end