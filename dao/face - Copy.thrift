
struct FaceInfo{
    1: i32 sno,
    2: string sname,
    3: string ssex,
    4: i16 sage,
    5: binary faceimg,
}

struct ScenePic{
    1: binary scene_img,
}

struct CameraInfo{
    1: string id,
    2: string position,
    3: string rtsp,
    4: string dvr_ip,
    5: i32    dvr_port,
    6: string dvr_user,
    7: string dvr_password,
    8: i32    dvr_channel
}

struct SnapFaceHis{
    1: string oid,
    2: string cameraid,
    3: string camera_pos,
    4: binary faceimg,
    5: i64 time,
}

struct PagedSnapFaceHis{
    1: i32         total_page,
    2: i32         total_count,
    3: list<SnapFaceHis> snap_face_his,
}

struct StatisTask{
    1: string start_camera_id,
    2: string end_camera_id,
    3: string start_camera_pos,
    4: string end_camera_pos,
}

struct FaceStatistics{
    1: i32 snap_total,
    2: i32 today_total,
}

struct Point{
    1: i32 x,
    2: i32 y,
}

struct AlarmHis{
    1: string oid,
    2: string camera_id,
    3: string camera_pos,
    4: string alarm_type,
    5: binary alarm_img,
    6: i64    time,
}

struct PagedAlarmHis{
    1: i32            total_page,
    2: i32 	      total_count,
    3: list<AlarmHis> alarm_his,
}


enum AreaType{
    FORBIDDENZONE = 0,
    ABDOOR,
}

struct Area{
    1: list<Point> points,
    2: AreaType area_type,
}

struct SearchFace{
    1: string oid,
    2: string camera_id,
    3: string camera_pos,
    4: double score,
    5: binary face_img,
    6: i64    time,
}

struct FaceLink{
    1: list<string> node_oids,
    2: list<list<string>> link_oids,
}

struct PageSnapFaceHis{
    1: string session_id,
    2: list<SnapFaceHis> snap_face_his,
}

service FaceServ{
    
    binary getFacePic(1:string oid),
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // get scene
     ScenePic getScenePic(1:string oid),
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // get alamscene
    ScenePic getAlarmScenePic(1:string oid),
    
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // camera info
    
    list<CameraInfo> getAllCameraInfo(),
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // history
    
    list<SnapFaceHis> snapHistory(1:string cameraid,2:i64 start_timestap,3:i64 end_timstap),
    
    PagedSnapFaceHis pagenateSnapHistory(1:i32 page,2:i32 num,3:string cameraid,4:i64 start_timestap,5:i64 end_timstap),
    
    PagedAlarmHis pagenateAlarmHistory(1:i32 page,2:i32 num,3:string camera_id,4:string alarm_type,5:i64 start_timestap,6:i64 end_timstap),
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Statis Task
    
    list<StatisTask> getStatisTask(),
    
    bool addStatisTask(1:string start_camera_id,2:string end_camera_id),
    
    bool deleteStatisTask(1:string start_camera_id,2:string end_camera_id),
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Alarm Area
    
    Area getAlarmArea(1:string camera_id),
    
    bool setAlarmArea(1:string camera_id,2:AreaType area_type,3:list<Point> area),
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Search Face 
    
    list<SearchFace> searchSnap(1:string oid,2:binary face_img, 3:string camera_id,4:i32 topk,5:double thresh,6:i64 start_timestap,7:i64 end_timstap),
    
    PageSnapFaceHis pagenateSearchFaceSnap(1:i32 page,2:i32 num,3:string oid,4:binary face_img, 5:string camera_id,6:i32 topk,7:double thresh,8:i64 start_timestap,9:i64 end_timstap),
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Generate
    
    list<SearchFace> gnerateFaceTrace(1:string oid,2:binary face_img, 3:double thresh,4:i64 start_timestap,5:i64 end_timstap),
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Generate FaceLink
    FaceLink generateFaceLink(1:string oid,2:binary face_img,3:i64 start_timestap,4:i64 end_timestap),
    
}
