
string readableTime(time_t);


string readableTime(time_t time)
{
	struct tm rt;
	localtime_s(&rt, &time);
	string datetime = str_pad(to_string(rt.tm_hour), 2, '0') + ":" + str_pad(to_string(rt.tm_min), 2, '0') + ":" + str_pad(to_string(rt.tm_sec), 2, '0') + " " + str_pad(to_string(rt.tm_mday), 2, '0') + "." + str_pad(to_string(rt.tm_mon + 1), 2, '0') + "." + str_pad(to_string(1900 + rt.tm_year), 4, '0');
	return datetime;
}
