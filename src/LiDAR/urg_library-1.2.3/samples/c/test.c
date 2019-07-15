/*!

*/

#include "urg_sensor.h"
#include "urg_utils.h"
#include "open_urg_sensor.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    urg_t urg;
    long *data;
    long max_distance;
    long min_distance;
    long time_stamp;
    int i;
    int j;
    int n;
    int first_step;
    int last_step;
    int skip_step;

    if (open_urg_sensor(&urg, argc, argv) < 0) {
        return 1;
    }


    // \~japanese 計測範囲を指定する
    // \~japanese センサ正面方向の 90 [deg] 範囲のデータ取得を行い、ステップ間引きを行わない例
    // \~english Defines the measurement scope (start, end steps)
    // \~english Defines a measurement scope of 90 [deg] at the front of the sensor, and no step grouping in this example
    first_step = urg_deg2step(&urg, -45);
    last_step = urg_deg2step(&urg, +45);
    skip_step = 0;
    urg_set_scanning_parameter(&urg, first_step, last_step, skip_step);

    data = (long *)malloc(urg_max_data_size(&urg) * sizeof(data[0]));
    if (!data) {
        perror("urg_max_index()");
        return 1;
    }

    // Gets measurement data
    urg_start_measurement(&urg, URG_DISTANCE, 10, 0);

    for (i = 0; i < 10; ++i) {
	 n = urg_get_distance(&urg, data, &time_stamp);
         printf("%ld\n", time_stamp);
	     urg_distance_min_max(&urg, &min_distance, &max_distance);
	    for (j = 0; j < n; ++j) {
		long distance = data[j];
		double radian;
		long x;
		long y;

		if ((distance < min_distance) || (distance > max_distance)) {
		    continue;
		}

		radian = urg_index2rad(&urg, j);
		x = (long)(distance * cos(radian));
		y = (long)(distance * sin(radian));

		printf("%ld, %ld\n", x, y);
	    }
	    printf("\n");
            
    }

    printf("\n");

    // Disconnects
free(data);
    urg_close(&urg);

#if defined(URG_MSC)
    getchar();
#endif
    return 0;
}
