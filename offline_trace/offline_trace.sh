scp -i /mnt/data/edu/cmu/spring_2018/RA/nvidia-volta.pem ubuntu@ec2volta.albertdavies.com:/var/lib/docker/volumes/my-vol/_data/Lock-based-sched/applications/Workzone_Detector/workzone_trace .
scp -i /mnt/data/edu/cmu/spring_2018/RA/nvidia-volta.pem ubuntu@ec2volta.albertdavies.com:/var/lib/docker/volumes/my-vol/_data/Lock-based-sched/applications/Lane_Change_Detector/lanechange_trace .
scp -i /mnt/data/edu/cmu/spring_2018/RA/nvidia-volta.pem ubuntu@ec2volta.albertdavies.com:/var/lib/docker/volumes/my-vol/_data/cuda_kernel/cuda_kernel_trace .
scp -i /mnt/data/edu/cmu/spring_2018/RA/nvidia-volta.pem ubuntu@ec2volta.albertdavies.com:/var/lib/docker/volumes/my-vol/_data/mod_dnn/all/tfClassifier/image_classification/dnn_trace .
scp -i /mnt/data/edu/cmu/spring_2018/RA/nvidia-volta.pem ubuntu@ec2volta.albertdavies.com:/var/lib/docker/volumes/my-vol/_data/cudaMon/cudamon_trace .
grep "[0-9].*cuda_kernel_high" cudamon_trace | cut -f 1,5 --output-delimiter=' ' | cut -c 7- > high_partition_trace
grep "[0-9].*cuda_kernel_low" cudamon_trace | cut -f 1,5 --output-delimiter=' ' | cut -c 7- > low_partition_trace
grep offline_trace_high cudamon_trace | cut -c 26- > high_trace
grep offline_trace_low cudamon_trace | cut -c 25- > low_trace
python live_graph_workzone.py
