function [t, x] = read_sim_log(logname)

data_log = fopen(logname, 'r');
data = fread(data_log, 'double');
data = reshape(data, 5, []);
fclose(data_log);

t = data(1,:);
x = data(2:5,:);

end