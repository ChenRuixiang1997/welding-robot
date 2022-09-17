// #include <iostream>
#include <transformation.h>
// PCL lib



//右手坐标系
void rotate_z(float x, float y, float z, float angle, float* x_output, float* y_output, float* z_output) 
{
    float atopi = angle / 180.0 * M_PI;
    *x_output = x * cos(atopi) - y * sin(atopi);
    *y_output = y * cos(atopi) + x * sin(atopi);
    *z_output = z;
 }

void rotate_x(float x, float y, float z, float angle, float* x_output, float* y_output, float* z_output)
{
    float atopi = angle / 180.0 * M_PI;
    *z_output = z * cos(atopi) + y * sin(atopi);
    *y_output = y * cos(atopi) - z * sin(atopi);
    *x_output = x;
 }

void rotate_y(float x, float y, float z, float angle, float* x_output, float* y_output, float* z_output)
{
    float atopi = angle / 180.0 * M_PI;
    *x_output = x * cos(atopi) - z * sin(atopi);
    *z_output = z * cos(atopi) + x * sin(atopi);
    *y_output = y;
 }



//四元数 -> 旋转矩阵
void Quaternion_to_RotationMatrix(float x, float y, float z, float w, float R[9])
{
  float r11 = 0, r12 = 0, r13 = 0;
  float r21 = 0, r22 = 0, r23 = 0; 
  float r31 = 0, r32 = 0, r33 = 0;

  R[0]  = 1 - 2 * y * y - 2 * z * z;
  R[1]  =     2 * x * y - 2 * z * w;
  R[2]  =     2 * x * z + 2 * y * w;

  R[3]  =     2 * x * y + 2 * z * w;
  R[4]  = 1 - 2 * x * x - 2 * z * z;
  R[5]  =     2 * y * z - 2 * x * w;

  R[6]  =     2 * x * z - 2 * y * w;
  R[7]  =     2 * y * z + 2 * x * w;
  R[8]  = 1 - 2 * x * x - 2 * y * y;

}

//欧拉角 -> 四元数
void euler_to_quaternion(float Yaw, float Pitch, float Roll, float Q[4])
{
  float yaw   = Yaw   * M_PI / 180 ;
  float pitch = Roll  * M_PI / 180 ;
  float roll  = Pitch * M_PI / 180 ;

  float qx = sin(roll/2) * cos(pitch/2) * cos(yaw/2) - cos(roll/2) * sin(pitch/2) * sin(yaw/2);
  float qy = cos(roll/2) * sin(pitch/2) * cos(yaw/2) + sin(roll/2) * cos(pitch/2) * sin(yaw/2);
  float qz = cos(roll/2) * cos(pitch/2) * sin(yaw/2) - sin(roll/2) * sin(pitch/2) * cos(yaw/2);
  float qw = cos(roll/2) * cos(pitch/2) * cos(yaw/2) + sin(roll/2) * sin(pitch/2) * sin(yaw/2);
 
  Q[0] = qx;
  Q[1] = qy;
  Q[2] = qz;
  Q[3] = qw;
}


//欧拉角 -> 旋转矩阵
void euler_to_RotationMatrix(float Yaw, float Pitch, float Roll, float R[9])
{
  float yaw   = Yaw   * M_PI / 180 ;
  float pitch = Roll  * M_PI / 180 ;
  float roll  = Pitch * M_PI / 180 ;

  float c1 = cos(yaw);
  float c2 = cos(pitch);
  float c3 = cos(roll);
  float s1 = sin(yaw);
  float s2 = sin(pitch);
  float s3 = sin(roll);

  R[0]  = c1 * c3 + s1 * s2 * s3;
  R[1]  = c3 * s1 * s2 - c1 * s3;
  R[2]  = c2 * s1;

  R[3]  = c2 * s3;
  R[4]  = c2 * c3;
  R[5]  = -s2;

  R[6]  = c1 * s2 * s3 - s1 * c3;
  R[7]  = s1 * s3 + c1 * c3 * s2;
  R[8]  = c1 * c2;
}

 
// pcl::PointXYZ camera_to_base_transform(geometry_msgs::Pose Base_End, pcl::PointXYZ Cam_Object)
// {

//   //Base -> End:
//   float T_B_E_Q[4];
//   T_B_E_Q[0] = Base_End.orientation.x;
//   T_B_E_Q[1] = Base_End.orientation.y;
//   T_B_E_Q[2] = Base_End.orientation.z;
//   T_B_E_Q[3] = Base_End.orientation.w;

//   float T_B_E_r[9];
//   Quaternion_to_RotationMatrix(T_B_E_Q[0], T_B_E_Q[1], T_B_E_Q[2], T_B_E_Q[3], T_B_E_r);   
//   // euler_to_RotationMatrix(0, -180, 0, T_B_E_r);
//   Matrix4d T_B_E; // Base -> End
//   T_B_E << T_B_E_r[0], T_B_E_r[1], T_B_E_r[2], Base_End.position.x,
//            T_B_E_r[3], T_B_E_r[4], T_B_E_r[5], Base_End.position.y,
//            T_B_E_r[6], T_B_E_r[7], T_B_E_r[8], Base_End.position.z,
//            0,          0,          0,          1;
//   // cout << "T_B_E: " << endl << T_B_E << endl;


//   // End -> Camera
//   Matrix4d T_E_C;
//   T_E_C <<   1,   2.7798e-18,            0,   -0.0355791,
//   -5.57321e-19,            1,  2.71051e-20,    -0.116142,
//     1.0842e-19,            0,            1,    0.0708987,
//              0,            0,            0,            1;
//   // cout << "T_E_C: " << endl << T_E_C << endl;


//   // Base -> Camera
//   Matrix4d T_B_C;
//   T_B_C = T_B_E * T_E_C;
//   // cout << "T_B_C: " << endl << T_B_C << endl;


//   // Camera -> object  0.0691944 -0.0164191
//   Matrix4d T_C_o; 
//   float T_C_o_Q[4];
//   euler_to_quaternion(0, 0, 0, T_C_o_Q);
//   float T_C_o_r[9];
//   Quaternion_to_RotationMatrix(T_C_o_Q[0], T_C_o_Q[1], T_C_o_Q[2], T_C_o_Q[3], T_C_o_r);//camera_color_optical_frame
//   // euler_to_RotationMatrix(-90, -180, 0, T_C_o_r);
//   T_C_o << T_C_o_r[0], T_C_o_r[1], T_C_o_r[2], Cam_Object.x,
//            T_C_o_r[3], T_C_o_r[4], T_C_o_r[5], Cam_Object.y,
//            T_C_o_r[6], T_C_o_r[7], T_C_o_r[8], Cam_Object.z,
//            0,          0,          0,          1;
//   // cout << "T_C_o: " << endl << T_C_o << endl;

//   // result by calibration:
//   Matrix4d T_R;
//   T_R = T_B_C * T_C_o;
//   // cout << "T_R: " << endl << T_R << endl;

//   pcl::PointXYZ object_position_world;
//   object_position_world.x = T_R(0,3);
//   object_position_world.y = T_R(1,3);
//   object_position_world.z = T_R(2,3);

//   return  object_position_world;
// }


// pcl::PointXYZ realsense_position_acquisition(tf::StampedTransform transform)
// {
//   geometry_msgs::Pose Base_End;
//   Base_End.position.x = transform.getOrigin().x();    Base_End.position.y = transform.getOrigin().y();    Base_End.position.z = transform.getOrigin().z();
//   Base_End.orientation.x = transform.getRotation().x(); Base_End.orientation.y = transform.getRotation().y(); Base_End.orientation.z = transform.getRotation().z(); Base_End.orientation.w = transform.getRotation().w();
//   // cout << "Base_End: " << Base_End << endl;
//  ///////////////////////////////////////////////////

//   //Base -> End:
//   float T_B_E_Q[4];
//   T_B_E_Q[0] = Base_End.orientation.x;
//   T_B_E_Q[1] = Base_End.orientation.y;
//   T_B_E_Q[2] = Base_End.orientation.z;
//   T_B_E_Q[3] = Base_End.orientation.w;

//   float T_B_E_r[9];
//   Quaternion_to_RotationMatrix(T_B_E_Q[0], T_B_E_Q[1], T_B_E_Q[2], T_B_E_Q[3], T_B_E_r);   
//   // euler_to_RotationMatrix(0, -180, 0, T_B_E_r);
//   Matrix4d T_B_E; // Base -> End
//   T_B_E << T_B_E_r[0], T_B_E_r[1], T_B_E_r[2], Base_End.position.x,
//            T_B_E_r[3], T_B_E_r[4], T_B_E_r[5], Base_End.position.y,
//            T_B_E_r[6], T_B_E_r[7], T_B_E_r[8], Base_End.position.z,
//            0,          0,          0,          1;
//   // cout << "T_B_E: " << endl << T_B_E << endl;


//   // End -> Camera
//   Matrix4d T_E_C;
//   T_E_C <<   1,   2.7798e-18,            0,   -0.0355791,
//   -5.57321e-19,            1,  2.71051e-20,    -0.106142,
//     1.0842e-19,            0,            1,    0.0708987,
//              0,            0,            0,            1;


//   // Base -> Camera
//   Matrix4d T_B_C;
//   T_B_C = T_B_E * T_E_C;
//   // cout << "T_B_C: " << endl << T_B_C << endl;

//   pcl::PointXYZ realsense_position;
//   realsense_position.x = T_B_C(0,3);
//   realsense_position.y = T_B_C(1,3);
//   realsense_position.z = T_B_C(2,3);

//   return realsense_position;
// }




// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void analyze_realsense_data(PointCloud::Ptr cloud)
// {
//   // 遍历深度图
//   for (int m = 0; m < depth_pic.rows; m++)  //480
//   {
//     for (int n = 0; n < depth_pic.cols; n++) //640
//     {
//       // 获取深度图中(m,n)处的值
//       float d = depth_pic.ptr<float>(m)[n]; 
//       // d 可能没有值，若如此，跳过此点
//       if (d == 0)
//           continue;

//       // d 存在值，则向点云增加一个点
//       pcl::PointXYZRGB p, p_z, p_x, p_y;
//       pcl::PointXYZRGB p_transform;

//       // 计算这个点的空间坐标
//       p.z = double(d) / camera_factor  ;
//       p.x = (n - camera_cx) * p.z / camera_fx; 
//       p.y = (m - camera_cy) * p.z / camera_fy;

//       //筛选位于机械臂工作空间内的点
//       if( (p.x < -1 || p.x > 1) || (p.y < -1 || p.y > 1) || (p.z < -1 || p.z > 1) )
//       {
//         continue;
//       }

//       // 从rgb图像中获取它的颜色
//       // rgb是三通道的BGR格式图，所以按下面的顺序获取颜色
//       p.b = color_pic.ptr<uchar>(m)[n*3];
//       p.g = color_pic.ptr<uchar>(m)[n*3+1];
//       p.r = color_pic.ptr<uchar>(m)[n*3+2];


//       cloud->points.push_back( p );        
//     }
//   }
// }




// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void coordinate_transformation(tf::StampedTransform transform, PointCloud::Ptr camera_pointcloud, PointCloud::Ptr cam_pc_transform, Cloud::Ptr cloud_ptr)
// {
//   geometry_msgs::Pose Base_End;
//   Base_End.position.x = transform.getOrigin().x();    Base_End.position.y = transform.getOrigin().y();    Base_End.position.z = transform.getOrigin().z();
//   Base_End.orientation.x = transform.getRotation().x(); Base_End.orientation.y = transform.getRotation().y(); Base_End.orientation.z = transform.getRotation().z(); Base_End.orientation.w = transform.getRotation().w();
//   ///////////////////////////////////////////////////////////////////////////////////////

//   for (float i = 0; i < camera_pointcloud->points.size(); i++)  //480
//   {
//     pcl::PointXYZRGB p_pushback; 
//     pcl::PointXYZ p_cloud_ptr;
//     pcl::PointXYZ Cam_Object;

//     Cam_Object.x    = camera_pointcloud->points[i].x; 
//     Cam_Object.y    = camera_pointcloud->points[i].y; 
//     Cam_Object.z    = camera_pointcloud->points[i].z; 

//     pcl::PointXYZ p_tranform = camera_to_base_transform(Base_End, Cam_Object);
    
//     p_cloud_ptr.x = p_pushback.x = p_tranform.x; 
//     p_cloud_ptr.y = p_pushback.y = p_tranform.y;  
//     p_cloud_ptr.z = p_pushback.z = p_tranform.z; 

//     p_pushback.b = camera_pointcloud->points[i].b;
//     p_pushback.g = camera_pointcloud->points[i].g;
//     p_pushback.r = camera_pointcloud->points[i].r;

//     cam_pc_transform->points.push_back( p_pushback );    

//     cloud_ptr->points.push_back( p_cloud_ptr );
//   }
// }



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
geometry_msgs::Pose Torch_to_End_transform(tf::StampedTransform transform_tool02torch)
{
  Eigen::Quaterniond q(transform_tool02torch.getRotation().w(), 
                       transform_tool02torch.getRotation().x(), 
                       transform_tool02torch.getRotation().y(),
                       transform_tool02torch.getRotation().z());

  Matrix3d RotationMatrix = q.toRotationMatrix();

  Matrix4d T_tool02torch;
  T_tool02torch << RotationMatrix(0,0), RotationMatrix(0,1), RotationMatrix(0,2), transform_tool02torch.getOrigin().x(),
                   RotationMatrix(1,0), RotationMatrix(1,1), RotationMatrix(1,2), transform_tool02torch.getOrigin().y(),
                   RotationMatrix(2,0), RotationMatrix(2,1), RotationMatrix(2,2), transform_tool02torch.getOrigin().z(),
                   0                  , 0                  , 0                  , 1;

  // Eigen::Vector3d euler_angles = RotationMatrix.eulerAngles(2, 1, 0) * 180 / M_PI; 
  // cout << "yaw(z) pitch(y) roll(x) = " << euler_angles.transpose() << endl;

  // cout << "T_tool02torch =\n" << T_tool02torch << endl;      


  // Eigen::AngleAxisd rotation_vector(RotationMatrix);
  // cout << "rotation_vector "  << "angle is: " << rotation_vector.angle() * (180 / M_PI) 
  //                             << " axis is: " << rotation_vector.axis().transpose() << endl;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Base_to_End_transform(int &receive_pose_flag, tf::StampedTransform transform)
{
  if (receive_pose_flag == 3)
  {
    geometry_msgs::Pose Base_End;
    Base_End.position.x = transform.getOrigin().x();    Base_End.position.y = transform.getOrigin().y();    Base_End.position.z = transform.getOrigin().z();
    Base_End.orientation.x = transform.getRotation().x(); Base_End.orientation.y = transform.getRotation().y(); Base_End.orientation.z = transform.getRotation().z(); Base_End.orientation.w = transform.getRotation().w();

    cout << "Base_End.position:" << Base_End.position.x << " " << Base_End.position.y << " " << Base_End.position.z << endl;
    // cout << "Base_End:\n" << Base_End << endl;
    receive_pose_flag = 0;
  }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Cloud::Ptr cloud_ptr_origin_copy(Cloud::Ptr cloud_ptr_new)
{
  Cloud::Ptr cloud_ptr_origin (new Cloud);
  for(float i = 0; i < cloud_ptr_new->points.size(); i++)
  {
    pcl::PointXYZ p;
    p.x = cloud_ptr_new->points[i].x;
    p.y = cloud_ptr_new->points[i].y;
    p.z = cloud_ptr_new->points[i].z;
    cloud_ptr_origin->points.push_back( p );    
  }

  return cloud_ptr_origin;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
tf::Transform Waypoint_markerTransform_creation(int i, geometry_msgs::Pose P)
{
  tf::Quaternion rotation (P.orientation.x,
                           P.orientation.y, 
                           P.orientation.z,
                           P.orientation.w);

  tf::Vector3 origin      (P.position.x, 
                           P.position.y,
                           P.position.z);

  tf::Transform t (rotation, origin);

  return t;
}

std::string Waypoint_markerName_creation( int i )
{
  std::string markerFrame = "waypoint_marker_";
  std::stringstream out;
  out << i;
  std::string id_string = out.str();
  markerFrame += id_string;

  return markerFrame;
}


// // Eigen::Quaterniond Transform_AngleAxisd_Quatenion(vector<Point3f> Normal_Vector)
// void Transform_AngleAxisd_Quatenion(Cloud::Ptr PathPoint_Position)
// {
//   vector<Point3f> move_Vector;
//   for(float i = 0; i < cloud_ptr->points.size() - 1; i++)
//   {
//     Point3f v;
//     if(cloud_ptr->points[i].x < cloud_ptr->points[i+1].x)
//     {
//       v.x = cloud_ptr->points[i+1].x - cloud_ptr->points[i].x;
//       v.y = cloud_ptr->points[i+1].y - cloud_ptr->points[i].y;
//     }
//     else
//     {
//       v.x = cloud_ptr->points[i].x - cloud_ptr->points[i+1].x;
//       v.y = cloud_ptr->points[i].y - cloud_ptr->points[i+1].y;
//     }
//     move_Vector.push_back(v);
//   }
//   cout << "move_Vector.size()" << move_Vector.size() << endl;

//   // for(int i = 0; i < Normal_Vector.size(); i++)
//   // {
//     // Eigen::AngleAxisd rotation_vector(-M_PI / 3, Eigen::Vector3d(0.020818098, -0.032153312, 0.99926615) );

//     // Eigen::AngleAxisd rotation_vector(M_PI / 4, Eigen::Vector3d(0, 1.0/sqrt(2), 1.0/sqrt(2)) );
//     // Eigen::Quaterniond q = Eigen::Quaterniond(rotation_vector);
//     // // cout << "quaternion = \n" << q.x() << " " << q.y() << " " << q.z() << " " << q.w() <<  endl;

//     // Eigen::Vector3d eulerAngle=rotation_vector.matrix().eulerAngles(2,1,0);


//   for(float i = 0; i < move_Vector.size() - 1; i++)
//   {
//     double x = move_Vector[i].x, y = move_Vector[i].y;//, z = -1.0/sqrt(2);
//     double pitch = 0, roll = 0, yaw = 0;

//     //yaw:
//     if(y > 0 && x > 0)
//       yaw = 0   + atan2(abs(y), abs(x)) * 180 / M_PI;
//     if(y > 0 && x < 0)
//       yaw = 90  + atan2(abs(x), abs(y)) * 180 / M_PI;
//     if(y < 0 && x < 0)
//       yaw = 180 + atan2(abs(y), abs(x)) * 180 / M_PI;
//     if(y < 0 && x > 0)
//       yaw = 270 + atan2(abs(x), abs(y)) * 180 / M_PI;

//     float q[4];
//     euler_to_quaternion(yaw, pitch, roll, q);

//     double px = 0.056;//double px = -0.04;
//     double py = 0.328;
//     double pz = 0.1;
//     float T_o_B_q[4];
//     double qx = q[0];
//     double qy = q[1];
//     double qz = q[2];
//     double qw = q[3];

//     tf::Quaternion rotation (qx,qy,qz,qw);
//     tf::Vector3 origin (px,py,pz);
//     tf::Transform t (rotation, origin);

//     std::string markerFrame = "GT_ar_marker_";
//     std::stringstream out;
//     out << 1;
//     std::string id_string = out.str();
//     markerFrame += id_string;
//     tf::StampedTransform GT_Marker (t, ros::Time::now(), "/base_link", markerFrame.c_str());
//     tf_broadcaster.sendTransform(GT_Marker);
//   }

//     // //pitch:
//     // if(y > 0 && z > 0)
//     //   pitch = 270 + atan2(abs(z), abs(y)) * 180 / M_PI;
//     // if(y > 0 && z < 0)
//     //   pitch = 180 + atan2(abs(y), abs(z)) * 180 / M_PI;
//     // if(y < 0 && z < 0)
//     //   pitch =  90 + atan2(abs(z), abs(y)) * 180 / M_PI;
//     // if(y < 0 && z > 0)
//     //   pitch =   0 + atan2(abs(y), abs(z)) * 180 / M_PI;

//     // //roll:
//     // if(y > 0 && z > 0)
//     //   pitch = 270 + atan2(abs(z), abs(y)) * 180 / M_PI;
//     // if(y > 0 && z < 0)
//     //   pitch = 180 + atan2(abs(y), abs(z)) * 180 / M_PI;
//     // if(y < 0 && z < 0)
//     //   pitch =  90 + atan2(abs(z), abs(y)) * 180 / M_PI;
//     // if(y < 0 && z > 0)
//     //   pitch =   0 + atan2(abs(y), abs(z)) * 180 / M_PI;

//     // //yaw:
//     // if(y > 0 && x > 0)
//     //   yaw = 0   + atan2(abs(y), abs(x)) * 180 / M_PI;
//     // if(y > 0 && x < 0)
//     //   yaw = 90  + atan2(abs(x), abs(y)) * 180 / M_PI;
//     // if(y < 0 && x < 0)
//     //   yaw = 180 + atan2(abs(y), abs(x)) * 180 / M_PI;
//     // if(y < 0 && x > 0)
//     //   yaw = 270 + atan2(abs(x), abs(y)) * 180 / M_PI;

//     // // cout << "eulerAngle: " << eulerAngle * 180 / M_PI << endl;
//     // cout << "eulerAngle: " <<  pitch << " " << roll << " " << yaw << endl;
//   // }

//     // double px = 0.056;//double px = -0.04;
//     // double py = 0.328;
//     // double pz = 0.1;
//     // float T_o_B_q[4];
//     // double qx = q.x();
//     // double qy = q.y();
//     // double qz = q.z();
//     // double qw = q.w();

//     // tf::Quaternion rotation (qx,qy,qz,qw);
//     // tf::Vector3 origin (px,py,pz);
//     // tf::Transform t (rotation, origin);

//     // std::string markerFrame = "GT_ar_marker_";
//     // std::stringstream out;
//     // out << 1;
//     // std::string id_string = out.str();
//     // markerFrame += id_string;
//     // tf::StampedTransform GT_Marker (t, ros::Time::now(), "/base_link", markerFrame.c_str());
//     // tf_broadcaster. sendTransform(GT_Marker);


//   return q;
// }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void pathpoint_cut_head_tail(int points_cut_count, 
                             Cloud::Ptr PathPoint_Position, 
                             vector<Point3f> Torch_Normal_Vector, 
                             Cloud::Ptr PathPoint_Position_final, 
                             Cloud::Ptr Torch_Normal_Vector_final)
{
  for(float i = points_cut_count; i < PathPoint_Position->points.size() - points_cut_count; i++)
  {
    pcl::PointXYZ p;
    p.x = PathPoint_Position->points[i].x;
    p.y = PathPoint_Position->points[i].y;
    p.z = PathPoint_Position->points[i].z;
    PathPoint_Position_final->points.push_back( p );    
  }
  for(float i = points_cut_count; i < Torch_Normal_Vector.size() - points_cut_count; i++)
  {
    pcl::PointXYZ  vector;
    vector.x = Torch_Normal_Vector[i].x;
    vector.y = Torch_Normal_Vector[i].y;
    vector.z = Torch_Normal_Vector[i].z;
    Torch_Normal_Vector_final->points.push_back(vector);
  }
  cout << "PathPoint_Position_final->points.size(): " << PathPoint_Position_final->points.size()  << endl;
  cout << "Torch_Normal_Vector_final.size(): "        << Torch_Normal_Vector_final->points.size() << endl << endl;

  for(float i = 0; i < Torch_Normal_Vector_final->points.size(); i++)
  {
    cout << "Torch_Normal_Vector_final"        << Torch_Normal_Vector_final->points[i] << endl;
  }

}

void X_Normal_Vector_vertical(Cloud::Ptr X_Normal_Vector,
                              Cloud::Ptr PathPoint_Position_final, 
                              Cloud::Ptr Torch_Normal_Vector_final)
{
  for(float i = 0; i < PathPoint_Position_final->points.size()-1; i++)
  {
    pcl::PointXYZ temp_vector;

    temp_vector.x = PathPoint_Position_final->points[i+1].x - PathPoint_Position_final->points[i].x;
    temp_vector.y = PathPoint_Position_final->points[i+1].y - PathPoint_Position_final->points[i].y;
    temp_vector.z = PathPoint_Position_final->points[i+1].z - PathPoint_Position_final->points[i].z;

    Vector3d v(                  temp_vector.x,                   temp_vector.y,                   temp_vector.z);
    Vector3d w(-Torch_Normal_Vector_final->points[i].x, -Torch_Normal_Vector_final->points[i].y, -Torch_Normal_Vector_final->points[i].z);
    Vector3d u = v.cross(w);
    float n = sqrt( u[0]*u[0] + u[1]*u[1] + u[2]*u[2] );

    pcl::PointXYZ x_normal_vector;
    x_normal_vector.x = u[0] / n;
    x_normal_vector.y = u[1] / n;
    x_normal_vector.z = u[2] / n; 
    
    X_Normal_Vector->points.push_back(x_normal_vector);

    if(i == PathPoint_Position_final->points.size()-1-1)
    {
      X_Normal_Vector->points.push_back(x_normal_vector);
    }
    // cout << "x_normal_vector" << x_normal_vector << endl;
  }

  // cout << "X_Normal_Vector: " << X_Normal_Vector->points.size() << endl << endl;
}

void Y_Z_Normal_Vector_vertical(Cloud::Ptr Y_Normal_Vector, 
                                Cloud::Ptr Z_Normal_Vector, 
                                Cloud::Ptr X_Normal_Vector, 
                                Cloud::Ptr Torch_Normal_Vector_final)
{
  for(float i = 0; i < X_Normal_Vector->points.size(); i++)
  {
    Vector3d v(-Torch_Normal_Vector_final->points[i].x, -Torch_Normal_Vector_final->points[i].y, -Torch_Normal_Vector_final->points[i].z);
    Vector3d w(           X_Normal_Vector->points[i].x,            X_Normal_Vector->points[i].y,            X_Normal_Vector->points[i].z);
    Vector3d u = v.cross(w);
    float n = sqrt( u[0]*u[0] + u[1]*u[1] + u[2]*u[2] );

    pcl::PointXYZ y_normal_vector;
    y_normal_vector.x = u[0] / n;
    y_normal_vector.y = u[1] / n;
    y_normal_vector.z = u[2] / n; 
    // cout << "y_normal_vector" << y_normal_vector << endl;
    Y_Normal_Vector->points.push_back(y_normal_vector);

    pcl::PointXYZ z_normal_vector;
    z_normal_vector.x = -Torch_Normal_Vector_final->points[i].x;
    z_normal_vector.y = -Torch_Normal_Vector_final->points[i].y;
    z_normal_vector.z = -Torch_Normal_Vector_final->points[i].z;
    
    Z_Normal_Vector->points.push_back(z_normal_vector);
  }
  // cout << "Y_Normal_Vector: " << Y_Normal_Vector->points.size()  << endl;
  // cout << "Z_Normal_Vector: " << Z_Normal_Vector->points.size()  << endl  << endl;

}

void direction_modification_vertical( Cloud::Ptr X_Normal_Vector,
                                      Cloud::Ptr Y_Normal_Vector, 
                                      Cloud::Ptr Z_Normal_Vector, 
                                      Cloud::Ptr PathPoint_Position_final)
{
  X_Normal_Vector->points.clear();
  for(float i = 0; i < Y_Normal_Vector->points.size(); i++)
  {
    Vector3d k(0-PathPoint_Position_final->points[i].x,
               0-PathPoint_Position_final->points[i].y,
               0-PathPoint_Position_final->points[i].z);

    Vector3d h(Y_Normal_Vector->points[i].x,
               Y_Normal_Vector->points[i].y,
               Y_Normal_Vector->points[i].z);

    if(k.dot(h) < 0)
    {
      Y_Normal_Vector->points[i].x = -Y_Normal_Vector->points[i].x;
      Y_Normal_Vector->points[i].y = -Y_Normal_Vector->points[i].y;
      Y_Normal_Vector->points[i].z = -Y_Normal_Vector->points[i].z;
    }

    Vector3d v(Y_Normal_Vector->points[i].x, Y_Normal_Vector->points[i].y, Y_Normal_Vector->points[i].z);
    Vector3d w(Z_Normal_Vector->points[i].x, Z_Normal_Vector->points[i].y, Z_Normal_Vector->points[i].z);
    Vector3d u = v.cross(w);
    float n = sqrt( u[0]*u[0] + u[1]*u[1] + u[2]*u[2] );

    pcl::PointXYZ x_normal_vector;
    x_normal_vector.x = u[0] / n;
    x_normal_vector.y = u[1] / n;
    x_normal_vector.z = u[2] / n; 
    
    X_Normal_Vector->points.push_back(x_normal_vector);

  }
  for(float i = 0; i < X_Normal_Vector->points.size(); i++)
  {
    // cout << "X_Normal_Vector: " << X_Normal_Vector->points[i] << endl;
  }
  cout << endl;
  for(float i = 0; i < Y_Normal_Vector->points.size(); i++)
  {
    // cout << "Y_Normal_Vector: " << Y_Normal_Vector->points[i] << endl;
  }
  cout << endl;
  for(float i = 0; i < Z_Normal_Vector->points.size(); i++)
  {
    // cout << "Z_Normal_Vector: " << Z_Normal_Vector->points[i] << endl;
  }
  cout << endl;
  cout << "X_Normal_Vector: " << X_Normal_Vector->points.size() << endl;
  cout << "Y_Normal_Vector: " << Y_Normal_Vector->points.size() << endl;
  cout << "Z_Normal_Vector: " << Z_Normal_Vector->points.size() << endl  << endl;
}

 

void Y_Normal_Vector_horizontal(Cloud::Ptr Y_Normal_Vector,
                                Cloud::Ptr PathPoint_Position_final, 
                                Cloud::Ptr Torch_Normal_Vector_final)
{
  for(float i = 0; i < PathPoint_Position_final->points.size()-1; i++)
  {
    pcl::PointXYZ temp_vector;

    temp_vector.x = PathPoint_Position_final->points[i+1].x - PathPoint_Position_final->points[i].x;
    temp_vector.y = PathPoint_Position_final->points[i+1].y - PathPoint_Position_final->points[i].y;
    temp_vector.z = PathPoint_Position_final->points[i+1].z - PathPoint_Position_final->points[i].z;

    Vector3d v(                  temp_vector.x,                   temp_vector.y,                   temp_vector.z);
    Vector3d w(-Torch_Normal_Vector_final->points[i].x, -Torch_Normal_Vector_final->points[i].y, -Torch_Normal_Vector_final->points[i].z);
    Vector3d u = w.cross(v);
    float n = sqrt( u[0]*u[0] + u[1]*u[1] + u[2]*u[2] );

    pcl::PointXYZ y_normal_vector;
    y_normal_vector.x = u[0] / n;
    y_normal_vector.y = u[1] / n;
    y_normal_vector.z = u[2] / n; 
    
    Y_Normal_Vector->points.push_back(y_normal_vector);

    if(i == PathPoint_Position_final->points.size()-1-1)
    {
      Y_Normal_Vector->points.push_back(y_normal_vector);
    }
    // cout << "y_normal_vector" << y_normal_vector << endl;
  }

  cout << "Y_Normal_Vector: " << Y_Normal_Vector->points.size() << endl << endl;
}

void X_Z_Normal_Vector_horizontal(Cloud::Ptr Y_Normal_Vector, 
                                  Cloud::Ptr Z_Normal_Vector, 
                                  Cloud::Ptr X_Normal_Vector, 
                                  Cloud::Ptr Torch_Normal_Vector_final)
{
  for(float i = 0; i < Y_Normal_Vector->points.size(); i++)
  {
    Vector3d v(-Torch_Normal_Vector_final->points[i].x, -Torch_Normal_Vector_final->points[i].y, -Torch_Normal_Vector_final->points[i].z);
    Vector3d w(           Y_Normal_Vector->points[i].x,            Y_Normal_Vector->points[i].y,            Y_Normal_Vector->points[i].z);
    Vector3d u = w.cross(v);
    float n = sqrt( u[0]*u[0] + u[1]*u[1] + u[2]*u[2] );

    pcl::PointXYZ x_normal_vector;
    x_normal_vector.x = u[0] / n;
    x_normal_vector.y = u[1] / n;
    x_normal_vector.z = u[2] / n; 
    // cout << "x_normal_vector" << x_normal_vector << endl;
    X_Normal_Vector->points.push_back(x_normal_vector);

    pcl::PointXYZ z_normal_vector;
    z_normal_vector.x = -Torch_Normal_Vector_final->points[i].x;
    z_normal_vector.y = -Torch_Normal_Vector_final->points[i].y;
    z_normal_vector.z = -Torch_Normal_Vector_final->points[i].z;
    
    Z_Normal_Vector->points.push_back(z_normal_vector);
  }
  cout << "X_Normal_Vector: " << X_Normal_Vector->points.size()  << endl;
  cout << "Z_Normal_Vector: " << Z_Normal_Vector->points.size()  << endl  << endl;

}

void Y_X_exchange(Cloud::Ptr Y_Normal_Vector, Cloud::Ptr X_Normal_Vector)
{
  int normal_count = Y_Normal_Vector->points.size();
  for(float i = 0; i < normal_count; i++)
  {
    
  }
}

void direction_modification_horizontal( Cloud::Ptr X_Normal_Vector,
                                        Cloud::Ptr Y_Normal_Vector, 
                                        Cloud::Ptr Z_Normal_Vector, 
                                        Cloud::Ptr PathPoint_Position_final)
{
  X_Normal_Vector->points.clear();
  for(float i = 0; i < Y_Normal_Vector->points.size(); i++)
  {
    Vector3d k(0,
               -1,
               0);

    Vector3d h(Y_Normal_Vector->points[i].x,
               Y_Normal_Vector->points[i].y,
               Y_Normal_Vector->points[i].z);

    if(k.dot(h) < 0)
    {
      Y_Normal_Vector->points[i].x = -Y_Normal_Vector->points[i].x;
      Y_Normal_Vector->points[i].y = -Y_Normal_Vector->points[i].y;
      Y_Normal_Vector->points[i].z = -Y_Normal_Vector->points[i].z;
    }

    Vector3d v(Y_Normal_Vector->points[i].x, Y_Normal_Vector->points[i].y, Y_Normal_Vector->points[i].z);
    Vector3d w(Z_Normal_Vector->points[i].x, Z_Normal_Vector->points[i].y, Z_Normal_Vector->points[i].z);
    Vector3d u = v.cross(w);
    float n = sqrt( u[0]*u[0] + u[1]*u[1] + u[2]*u[2] );

    pcl::PointXYZ x_normal_vector;
    x_normal_vector.x = u[0] / n;
    x_normal_vector.y = u[1] / n;
    x_normal_vector.z = u[2] / n; 
    
    X_Normal_Vector->points.push_back(x_normal_vector);
  }

  for(float i = 0; i < X_Normal_Vector->points.size(); i++)
  {
    // cout << "X_Normal_Vector: " << X_Normal_Vector->points[i] << endl;
  }
  cout << endl;
  for(float i = 0; i < Y_Normal_Vector->points.size(); i++)
  {
    // cout << "Y_Normal_Vector: " << Y_Normal_Vector->points[i] << endl;
  }
  cout << endl;
  for(float i = 0; i < Z_Normal_Vector->points.size(); i++)
  {
    // cout << "Z_Normal_Vector: " << Z_Normal_Vector->points[i] << endl;
  }
  cout << endl;
  cout << "X_Normal_Vector: " << X_Normal_Vector->points.size() << endl;
  cout << "Y_Normal_Vector: " << Y_Normal_Vector->points.size() << endl;
  cout << "Z_Normal_Vector: " << Z_Normal_Vector->points.size() << endl  << endl;
}


void Temp_vector_computation( Cloud::Ptr Temp_vector1,
                              Cloud::Ptr Temp_vector2,
                              Cloud::Ptr PathPoint_Position_final, 
                              Cloud::Ptr Torch_Normal_Vector_final)
{
  for(float i = 0; i < PathPoint_Position_final->points.size()-1; i++)
  {
    pcl::PointXYZ temp_vector;

    temp_vector.x = PathPoint_Position_final->points[i+1].x - PathPoint_Position_final->points[i].x;
    temp_vector.y = PathPoint_Position_final->points[i+1].y - PathPoint_Position_final->points[i].y;
    temp_vector.z = PathPoint_Position_final->points[i+1].z - PathPoint_Position_final->points[i].z;

    Vector3d v(                  temp_vector.x,                   temp_vector.y,                   temp_vector.z);
    Vector3d w(-Torch_Normal_Vector_final->points[i].x, -Torch_Normal_Vector_final->points[i].y, -Torch_Normal_Vector_final->points[i].z);
    Vector3d u = w.cross(v);
    float n = sqrt( u[0]*u[0] + u[1]*u[1] + u[2]*u[2] );

    pcl::PointXYZ vec;
    vec.x = u[0] / n;
    vec.y = u[1] / n;
    vec.z = u[2] / n; 
    
    Temp_vector1->points.push_back(vec);

    if(i == PathPoint_Position_final->points.size()-1-1)
    {
      Temp_vector1->points.push_back(vec);
    }
    // cout << "vec" << vec << endl;
  }

  cout << "Temp_vector1: " << Temp_vector1->points.size() << endl << endl;
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  for(float i = 0; i < Temp_vector1->points.size(); i++)
  {
    Vector3d v(-Torch_Normal_Vector_final->points[i].x, -Torch_Normal_Vector_final->points[i].y, -Torch_Normal_Vector_final->points[i].z);
    Vector3d w(              Temp_vector1->points[i].x,               Temp_vector1->points[i].y,               Temp_vector1->points[i].z);
    Vector3d u = w.cross(v);
    float n = sqrt( u[0]*u[0] + u[1]*u[1] + u[2]*u[2] );

    pcl::PointXYZ vec;
    vec.x = u[0] / n;
    vec.y = u[1] / n;
    vec.z = u[2] / n; 
    // cout << "x_normal_vector" << x_normal_vector << endl;
    Temp_vector2->points.push_back(vec);

  }
  cout << "Temp_vector2: " << Temp_vector2->points.size() << endl << endl;
}

void Y_Normal_Vector_computation(Cloud::Ptr Y_Normal_Vector,
                                 Cloud::Ptr Temp_vector1,
                                 Cloud::Ptr Temp_vector2)
{
  int vec_count = Temp_vector1->points.size();

  for(float i = 0; i < Temp_vector1->points.size(); i++)
  {
    Vector3d k(0,
               -1,
               0);

    Vector3d vec1(Temp_vector1->points[i].x,
                  Temp_vector1->points[i].y,
                  Temp_vector1->points[i].z);

    Vector3d vec2(Temp_vector2->points[i].x,
                  Temp_vector2->points[i].y,
                  Temp_vector2->points[i].z);

    if( fabs(k.dot(vec1)) >= fabs(k.dot(vec2)) )
    {
      Y_Normal_Vector->points.push_back(Temp_vector1->points[i]);
    }
    if( fabs(k.dot(vec1)) < fabs(k.dot(vec2)) )
    {
      Y_Normal_Vector->points.push_back(Temp_vector2->points[i]);
    }
  }
  cout << "Y_Normal_Vector: " << Y_Normal_Vector->points.size() << endl << endl;

}


void X_Z_Normal_Vector_computation(Cloud::Ptr Y_Normal_Vector, 
                                   Cloud::Ptr Z_Normal_Vector, 
                                   Cloud::Ptr X_Normal_Vector, 
                                   Cloud::Ptr Torch_Normal_Vector_final)
{
  for(float i = 0; i < Y_Normal_Vector->points.size(); i++)
  {
    Vector3d v(-Torch_Normal_Vector_final->points[i].x, -Torch_Normal_Vector_final->points[i].y, -Torch_Normal_Vector_final->points[i].z);
    Vector3d w(           Y_Normal_Vector->points[i].x,            Y_Normal_Vector->points[i].y,            Y_Normal_Vector->points[i].z);
    Vector3d u = w.cross(v);
    float n = sqrt( u[0]*u[0] + u[1]*u[1] + u[2]*u[2] );

    pcl::PointXYZ x_normal_vector;
    x_normal_vector.x = u[0] / n;
    x_normal_vector.y = u[1] / n;
    x_normal_vector.z = u[2] / n; 
    // cout << "x_normal_vector" << x_normal_vector << endl;
    X_Normal_Vector->points.push_back(x_normal_vector);

    pcl::PointXYZ z_normal_vector;
    z_normal_vector.x = -Torch_Normal_Vector_final->points[i].x;
    z_normal_vector.y = -Torch_Normal_Vector_final->points[i].y;
    z_normal_vector.z = -Torch_Normal_Vector_final->points[i].z;
    
    Z_Normal_Vector->points.push_back(z_normal_vector);
    cout << "z_normal_vector: " << z_normal_vector << endl;
  }
  cout << "X_Normal_Vector: " << X_Normal_Vector->points.size()  << endl;
  cout << "Z_Normal_Vector: " << Z_Normal_Vector->points.size()  << endl  << endl;

}

 
void direction_modification(Cloud::Ptr X_Normal_Vector,
                            Cloud::Ptr Y_Normal_Vector, 
                            Cloud::Ptr Z_Normal_Vector, 
                            Cloud::Ptr PathPoint_Position_final)
{
  X_Normal_Vector->points.clear();
  for(float i = 0; i < Y_Normal_Vector->points.size(); i++)
  {
    Vector3d k(0,
               -1,
               0);

    Vector3d h(Y_Normal_Vector->points[i].x,
               Y_Normal_Vector->points[i].y,
               Y_Normal_Vector->points[i].z);

    if(k.dot(h) < 0)
    {
      Y_Normal_Vector->points[i].x = -Y_Normal_Vector->points[i].x;
      Y_Normal_Vector->points[i].y = -Y_Normal_Vector->points[i].y;
      Y_Normal_Vector->points[i].z = -Y_Normal_Vector->points[i].z;
    }

    Vector3d v(Y_Normal_Vector->points[i].x, Y_Normal_Vector->points[i].y, Y_Normal_Vector->points[i].z);
    Vector3d w(Z_Normal_Vector->points[i].x, Z_Normal_Vector->points[i].y, Z_Normal_Vector->points[i].z);
    Vector3d u = v.cross(w);
    float n = sqrt( u[0]*u[0] + u[1]*u[1] + u[2]*u[2] );

    pcl::PointXYZ x_normal_vector;
    x_normal_vector.x = u[0] / n;
    x_normal_vector.y = u[1] / n;
    x_normal_vector.z = u[2] / n; 
    
    X_Normal_Vector->points.push_back(x_normal_vector);
  }

  for(float i = 0; i < X_Normal_Vector->points.size(); i++)
  {
    // cout << "X_Normal_Vector: " << X_Normal_Vector->points[i] << endl;
  }
  cout << endl;
  for(float i = 0; i < Y_Normal_Vector->points.size(); i++)
  {
    // cout << "Y_Normal_Vector: " << Y_Normal_Vector->points[i] << endl;
  }
  cout << endl;
  for(float i = 0; i < Z_Normal_Vector->points.size(); i++)
  {
    // cout << "Z_Normal_Vector: " << Z_Normal_Vector->points[i] << endl;
  }
  cout << endl;
  cout << "X_Normal_Vector: " << X_Normal_Vector->points.size() << endl;
  cout << "Y_Normal_Vector: " << Y_Normal_Vector->points.size() << endl;
  cout << "Z_Normal_Vector: " << Z_Normal_Vector->points.size() << endl  << endl;
}





vector< geometry_msgs::Pose >  Moveit_Pose_generation(float trajectory_point_size, 
                                                      Cloud::Ptr X_Normal_Vector, 
                                                      Cloud::Ptr Y_Normal_Vector, 
                                                      Cloud::Ptr Z_Normal_Vector, 
                                                      Cloud::Ptr PathPoint_Position_final)
{
  vector< geometry_msgs::Pose > Rviz_TrajectoryPose;
  for(float i = 0; i < trajectory_point_size; i++)
  {
    geometry_msgs::Pose pose;
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 旋转矩阵转换为四元数
    Eigen::Matrix3d origin_base;
    origin_base << 1, 0, 0,
                   0, 1, 0, 
                   0, 0, 1;
    
    Eigen::Matrix3d transformed_base;
    transformed_base << X_Normal_Vector->points[i].x, Y_Normal_Vector->points[i].x, Z_Normal_Vector->points[i].x,
                        X_Normal_Vector->points[i].y, Y_Normal_Vector->points[i].y, Z_Normal_Vector->points[i].y, 
                        X_Normal_Vector->points[i].z, Y_Normal_Vector->points[i].z, Z_Normal_Vector->points[i].z;

    Eigen::Matrix3d rotation_matrix;
    rotation_matrix = origin_base * transformed_base;
    
    Eigen::Quaterniond rotation(rotation_matrix);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    pose.position.x = PathPoint_Position_final->points[i].x;
    pose.position.y = PathPoint_Position_final->points[i].y;
    pose.position.z = PathPoint_Position_final->points[i].z;
    pose.orientation.x = rotation.x();
    pose.orientation.y = rotation.y();
    pose.orientation.z = rotation.z();
    pose.orientation.w = rotation.w();
    Rviz_TrajectoryPose.push_back(pose);

  }
  // cout << "Rviz_TrajectoryPose.size(): " << Rviz_TrajectoryPose.size() << endl << endl;

  return Rviz_TrajectoryPose;
}

void URx_Pose_generation( float trajectory_point_size, 
                          Cloud::Ptr X_Normal_Vector, 
                          Cloud::Ptr Y_Normal_Vector, 
                          Cloud::Ptr Z_Normal_Vector, 
                          Cloud::Ptr PathPoint_Position_final,
                          vector< geometry_msgs::Pose > &Welding_Trajectory)
{
  for(float i = 0; i < trajectory_point_size; i++)
  {
    // cout << "count: " << i+1 << endl;

    //for URx：
    // 已知两个坐标系的各自基向量，求他们之间的旋转矩阵
    // 坐标系构造方法： x y z，竖直排列
    Eigen::Matrix3d origin_base_URx;
    origin_base_URx << -1,  0, 0,
                        0, -1, 0, 
                        0,  0, 1;
    
    Eigen::Matrix3d transformed_base_URx;
    transformed_base_URx << X_Normal_Vector->points[i].x, Y_Normal_Vector->points[i].x, Z_Normal_Vector->points[i].x,
                            X_Normal_Vector->points[i].y, Y_Normal_Vector->points[i].y, Z_Normal_Vector->points[i].y, 
                            X_Normal_Vector->points[i].z, Y_Normal_Vector->points[i].z, Z_Normal_Vector->points[i].z;


    Eigen::Matrix3d rotation_matrix_URx;
    rotation_matrix_URx = origin_base_URx * transformed_base_URx;

    Matrix4d T_Base2Torch;
    T_Base2Torch << rotation_matrix_URx(0,0), rotation_matrix_URx(0,1), rotation_matrix_URx(0,2), -PathPoint_Position_final->points[i].x,
                    rotation_matrix_URx(1,0), rotation_matrix_URx(1,1), rotation_matrix_URx(1,2), -PathPoint_Position_final->points[i].y,
                    rotation_matrix_URx(2,0), rotation_matrix_URx(2,1), rotation_matrix_URx(2,2),  PathPoint_Position_final->points[i].z,
                    0                      , 0                       , 0                       , 1;

    Matrix4d T_End2Torch;
    T_End2Torch << 1,  0,        0,        0,
                   0,  0.660006, 0.751261, 0.048,
                  -0, -0.751261, 0.660006, 0.227,
                   0,  0,        0,        1;

    Matrix4d T_Base2End = T_Base2Torch * T_End2Torch.inverse();

    Eigen::Matrix3d End_rotation_matrix_URx;
    End_rotation_matrix_URx << T_Base2End(0,0), T_Base2End(0,1), T_Base2End(0,2),
                               T_Base2End(1,0), T_Base2End(1,1), T_Base2End(1,2),
                               T_Base2End(2,0), T_Base2End(2,1), T_Base2End(2,2);

    Eigen::AngleAxisd End_rotation_vector_URx(End_rotation_matrix_URx);
    // cout << "End_rotation_vector_URx "  << "angle is: " << End_rotation_vector_URx.angle() * (180 / M_PI) 
    //                                     << " axis is: " << End_rotation_vector_URx.axis().transpose() << endl;

    ////////////////////////////////////////////////////////////////////////////
    geometry_msgs::Pose pose;
    pose.position.x    = T_Base2End(0,3);
    pose.position.y    = T_Base2End(1,3);
    pose.position.z    = T_Base2End(2,3);
    pose.orientation.x = End_rotation_vector_URx.angle() * End_rotation_vector_URx.axis().x(); 
    pose.orientation.y = End_rotation_vector_URx.angle() * End_rotation_vector_URx.axis().y(); 
    pose.orientation.z = End_rotation_vector_URx.angle() * End_rotation_vector_URx.axis().z(); 
    pose.orientation.w = 0;
    // cout << "welding_trajectory_pose" << pose << endl;
    Welding_Trajectory.push_back(pose);
  }
}


Eigen::Quaterniond rotation_Quaternionslerp(Eigen::Quaterniond starting, Eigen::Quaterniond ending, float t )
{
    float cosa = starting.x()*ending.x() + starting.y()*ending.y() + starting.z()*ending.z() + starting.w()*ending.w();
    
    // If the dot product is negative, the quaternions have opposite handed-ness and slerp won't take
    // the shorter path. Fix by reversing one quaternion.
    Eigen::Quaterniond ending_copy;
    if ( cosa < 0.0f ) 
    {
        ending_copy.x() = -ending.x();
        ending_copy.y() = -ending.y();
        ending_copy.z() = -ending.z();
        ending_copy.w() = -ending.z();
        cosa = -cosa;
    }
    
    float k0 = 0, k1 = 0;
    
    // If the inputs are too close for comfort, linearly interpolate
    if ( cosa > 0.9995f ) 
    {
        k0 = 1.0f - t;
        k1 = t;
    }
    else 
    {
        float sina = sqrt( 1.0f - cosa*cosa );
        float a = atan2( sina, cosa );
        k0 = sin((1.0f - t)*a)  / sina;
        k1 = sin(t*a) / sina;
    }

    Eigen::Quaterniond result;
    result.x() = starting.x()*k0 + ending_copy.x()*k1;
    result.y() = starting.y()*k0 + ending_copy.y()*k1;
    result.z() = starting.z()*k0 + ending_copy.z()*k1;
    result.w() = starting.w()*k0 + ending_copy.w()*k1;

    return result;
}

vector< geometry_msgs::Pose > RvizPose_pointInterpolation( int origin_pathpoint_size,
                                                           PointCloud::Ptr rotation_originWaypoint, 
                                                           Cloud::Ptr Pathpoint_Temp)
{
 // vector < vector< geometry_msgs::Pose > > Pose_all;
  vector< geometry_msgs::Pose > Rviz_TrajectoryPose;
  float dis_finalPoints = 0.0001;
  for(float i = 0; i < origin_pathpoint_size - 1; i++)
  {
    // float dis_originPoints = sqrt( pow(Pathpoint_Temp[i].x - Pathpoint_Temp[i+1].x, 2) + 
    //                                pow(Pathpoint_Temp[i].y - Pathpoint_Temp[i+1].y, 2) +
    //                                pow(Pathpoint_Temp[i].z - Pathpoint_Temp[i+1].z, 2));
    
    // int points_size = dis_originPoints / dis_finalPoints;
    // cout << "points_size: " << points_size << endl;

    // Point3f pair_vector;
    // pair_vector.x = Pathpoint_Temp[i+1].x - Pathpoint_Temp[i].x;
    // pair_vector.y = Pathpoint_Temp[i+1].y - Pathpoint_Temp[i].y;
    // pair_vector.z = Pathpoint_Temp[i+1].z - Pathpoint_Temp[i].z;

    // float n = sqrt( pair_vector.x*pair_vector.x + pair_vector.y*pair_vector.y + pair_vector.z*pair_vector.z );
    
    // Point3f pair_vector_scaled;
    // pair_vector_scaled.x = pair_vector.x / points_size;
    // pair_vector_scaled.y = pair_vector.y / points_size;
    // pair_vector_scaled.z = pair_vector.z / points_size;

    // for(float j = 0; j < points_size; j++)
    // {
    //   // Eigen::Quaterniond rotation = rotation_Quaternionslerp(rotation_originWaypoint[i], rotation_originWaypoint[i+1], j * 1.0 / points_size);
    //   Eigen::Quaterniond rotation = rotation_originWaypoint[i];
    //   geometry_msgs::Pose pose;
    //   pose.position.x    = j * pair_vector_scaled.x + Pathpoint_Temp[i].x;
    //   pose.position.y    = j * pair_vector_scaled.y + Pathpoint_Temp[i].y;
    //   pose.position.z    = j * pair_vector_scaled.z + Pathpoint_Temp[i].z;
    //   pose.orientation.x = rotation.x();
    //   pose.orientation.y = rotation.y();
    //   pose.orientation.z = rotation.z();
    //   pose.orientation.w = rotation.w();

    //   Rviz_TrajectoryPose.push_back(pose);
    // }

    ////////////////////////////////////////////////////////////////////////////////////////////

    geometry_msgs::Pose pose;
    pose.position.x    =          Pathpoint_Temp->points[i].x;
    pose.position.y    =          Pathpoint_Temp->points[i].y;
    pose.position.z    =          Pathpoint_Temp->points[i].z;
    pose.orientation.x = rotation_originWaypoint->points[i].x;
    pose.orientation.y = rotation_originWaypoint->points[i].y;
    pose.orientation.z = rotation_originWaypoint->points[i].z;
    pose.orientation.w = rotation_originWaypoint->points[i].r;
    Rviz_TrajectoryPose.push_back(pose);


    if(i == origin_pathpoint_size - 2)
    {
      geometry_msgs::Pose pose;
      pose.position.x    =          Pathpoint_Temp->points[i+1].x;
      pose.position.y    =          Pathpoint_Temp->points[i+1].y;
      pose.position.z    =          Pathpoint_Temp->points[i+1].z;
      pose.orientation.x = rotation_originWaypoint->points[i+1].x;
      pose.orientation.y = rotation_originWaypoint->points[i+1].y;
      pose.orientation.z = rotation_originWaypoint->points[i+1].z;
      pose.orientation.w = rotation_originWaypoint->points[i+1].r;
      Rviz_TrajectoryPose.push_back(pose);
    }
 


  }
  // for(float i = 0; i < Rviz_TrajectoryPose.size(); i++)
  // {
  //   cout <<  Rviz_TrajectoryPose[i] << endl;
  // }

  // cout << "Rviz_TrajectoryPose.size(): " << Rviz_TrajectoryPose.size() << endl;

  return Rviz_TrajectoryPose;
}


void orientation_definition_verticalType( Cloud::Ptr X_Normal_Vector,
                                          Cloud::Ptr Y_Normal_Vector, 
                                          Cloud::Ptr Z_Normal_Vector, 
                                          Cloud::Ptr PathPoint_Position_final, 
                                          Cloud::Ptr Torch_Normal_Vector_final)

{
  X_Normal_Vector_vertical( X_Normal_Vector,
                            PathPoint_Position_final, 
                            Torch_Normal_Vector_final);

  Y_Z_Normal_Vector_vertical( Y_Normal_Vector, 
                              Z_Normal_Vector, 
                              X_Normal_Vector, 
                              Torch_Normal_Vector_final);

  direction_modification_vertical(X_Normal_Vector, 
                                  Y_Normal_Vector, 
                                  Z_Normal_Vector, 
                                  PathPoint_Position_final);
}

void orientation_definition_horizontalType( Cloud::Ptr X_Normal_Vector,
                                            Cloud::Ptr Y_Normal_Vector, 
                                            Cloud::Ptr Z_Normal_Vector, 
                                            Cloud::Ptr PathPoint_Position_final, 
                                            Cloud::Ptr Torch_Normal_Vector_final)

{
  Y_Normal_Vector_horizontal( Y_Normal_Vector,
                              PathPoint_Position_final, 
                              Torch_Normal_Vector_final);

  X_Z_Normal_Vector_horizontal( Y_Normal_Vector, 
                                Z_Normal_Vector, 
                                X_Normal_Vector, 
                                Torch_Normal_Vector_final);

  direction_modification_horizontal(X_Normal_Vector, 
                                    Y_Normal_Vector, 
                                    Z_Normal_Vector, 
                                    PathPoint_Position_final);
}


void orientation_computation( Cloud::Ptr X_Normal_Vector,
                              Cloud::Ptr Y_Normal_Vector, 
                              Cloud::Ptr Z_Normal_Vector, 
                              Cloud::Ptr PathPoint_Position_final, 
                              Cloud::Ptr Torch_Normal_Vector_final)

{
  Cloud::Ptr Temp_vector1 (new Cloud);   
  Cloud::Ptr Temp_vector2 (new Cloud); 

  Temp_vector_computation(Temp_vector1,
                          Temp_vector2,
                          PathPoint_Position_final,
                          Torch_Normal_Vector_final);

  Y_Normal_Vector_computation(Y_Normal_Vector,
                              Temp_vector1, 
                              Temp_vector2);

  X_Z_Normal_Vector_computation(Y_Normal_Vector, 
                                Z_Normal_Vector, 
                                X_Normal_Vector, 
                                Torch_Normal_Vector_final);


  int intern = 3;
  Cloud::Ptr X_Normal_Vector_filtered (new Cloud); 
  
  for(float i = 0; i < X_Normal_Vector->points.size(); i++)
  {
    pcl::PointXYZ x_normal_vector_filtered;

    if(i >= 0 + intern && i <= X_Normal_Vector->points.size() - 1 - intern)
    {
      for(float j = i - intern; j <= i + intern; j++)
      {
        x_normal_vector_filtered.x += X_Normal_Vector->points[j].x;
        x_normal_vector_filtered.y += X_Normal_Vector->points[j].y;
        x_normal_vector_filtered.z += X_Normal_Vector->points[j].z;
      }

      x_normal_vector_filtered.x = x_normal_vector_filtered.x / (2 * intern + 1) * 1.0;
      x_normal_vector_filtered.y = x_normal_vector_filtered.y / (2 * intern + 1) * 1.0;
      x_normal_vector_filtered.z = x_normal_vector_filtered.z / (2 * intern + 1) * 1.0;
    }

    if(i < intern)
    {
      for(float j = 0; j <= 0 + 2 * intern; j++)
      {
        x_normal_vector_filtered.x += X_Normal_Vector->points[j].x;
        x_normal_vector_filtered.y += X_Normal_Vector->points[j].y;
        x_normal_vector_filtered.z += X_Normal_Vector->points[j].z;
      }

      x_normal_vector_filtered.x = x_normal_vector_filtered.x / (2 * intern + 1) * 1.0;
      x_normal_vector_filtered.y = x_normal_vector_filtered.y / (2 * intern + 1) * 1.0;
      x_normal_vector_filtered.z = x_normal_vector_filtered.z / (2 * intern + 1) * 1.0;
    }


    if(i > X_Normal_Vector->points.size() - 1 - intern)
    {
      for(float j = X_Normal_Vector->points.size() - 1 - 2 * intern; j <= X_Normal_Vector->points.size() - 1; j++)
      {
        x_normal_vector_filtered.x += X_Normal_Vector->points[j].x;
        x_normal_vector_filtered.y += X_Normal_Vector->points[j].y;
        x_normal_vector_filtered.z += X_Normal_Vector->points[j].z;
      }

      x_normal_vector_filtered.x = x_normal_vector_filtered.x / (2 * intern + 1) * 1.0;
      x_normal_vector_filtered.y = x_normal_vector_filtered.y / (2 * intern + 1) * 1.0;
      x_normal_vector_filtered.z = x_normal_vector_filtered.z / (2 * intern + 1) * 1.0;
    }

    X_Normal_Vector_filtered->points.push_back(x_normal_vector_filtered);

  }
  X_Normal_Vector->points.clear();

  for(float i = 0; i < X_Normal_Vector_filtered->points.size(); i++)
  {
    pcl::PointXYZ p;

    p.x = X_Normal_Vector_filtered->points[i].x;
    p.y = X_Normal_Vector_filtered->points[i].y;
    p.z = X_Normal_Vector_filtered->points[i].z;
    X_Normal_Vector->points.push_back(p);
  }

  Y_Normal_Vector->points.clear();
  for(float i = 0; i < X_Normal_Vector->points.size(); i++)
  {
    Vector3d v(X_Normal_Vector->points[i].x, X_Normal_Vector->points[i].y, X_Normal_Vector->points[i].z);
    Vector3d w(Z_Normal_Vector->points[i].x, Z_Normal_Vector->points[i].y, Z_Normal_Vector->points[i].z);
    Vector3d u = w.cross(v);
    float n = sqrt( u[0]*u[0] + u[1]*u[1] + u[2]*u[2] );

    pcl::PointXYZ y_normal_vector;
    y_normal_vector.x = u[0] / n;
    y_normal_vector.y = u[1] / n;
    y_normal_vector.z = u[2] / n; 
    
    Y_Normal_Vector->points.push_back(y_normal_vector);
  }


  direction_modification( X_Normal_Vector, 
                          Y_Normal_Vector, 
                          Z_Normal_Vector, 
                          PathPoint_Position_final);
}
















vector< geometry_msgs::Pose > Ultimate_6DOF_TrajectoryGeneration(vector< geometry_msgs::Pose > &Welding_Trajectory, 
                                                                 Cloud::Ptr PathPoint_Position, 
                                                                 vector<Point3f> Torch_Normal_Vector)
{
  int points_cut_count = 3;
  Cloud::Ptr PathPoint_Position_final  (new Cloud);   
  Cloud::Ptr Torch_Normal_Vector_final (new Cloud);  
  pathpoint_cut_head_tail(points_cut_count, 
                          PathPoint_Position, 
                          Torch_Normal_Vector, 
                          PathPoint_Position_final, 
                          Torch_Normal_Vector_final);

  // ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Cloud::Ptr X_Normal_Vector  (new Cloud);   
  Cloud::Ptr Y_Normal_Vector  (new Cloud);   
  Cloud::Ptr Z_Normal_Vector  (new Cloud);   

  // orientation_definition_verticalType( X_Normal_Vector, 
  //                                      Y_Normal_Vector, 
  //                                      Z_Normal_Vector, 
  //                                      PathPoint_Position_final, 
  //                                      Torch_Normal_Vector_final);

  // orientation_definition_horizontalType(X_Normal_Vector, 
  //                                       Y_Normal_Vector, 
  //                                       Z_Normal_Vector, 
  //                                       PathPoint_Position_final, 
  //                                       Torch_Normal_Vector_final);
  orientation_computation(X_Normal_Vector, 
                          Y_Normal_Vector, 
                          Z_Normal_Vector, 
                          PathPoint_Position_final, 
                          Torch_Normal_Vector_final);

  // //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  float trajectory_point_size = PathPoint_Position_final->points.size();

  // PointCloud::Ptr rotation_originWaypoint (new PointCloud);
  // Cloud::Ptr Pathpoint_Temp (new Cloud);  
  vector< geometry_msgs::Pose > Rviz_TrajectoryPose = Moveit_Pose_generation( trajectory_point_size, 
                                                                              X_Normal_Vector, 
                                                                              Y_Normal_Vector, 
                                                                              Z_Normal_Vector, 
                                                                              PathPoint_Position_final);
 
  // cout << "rotation_originWaypoint.size(): "  << rotation_originWaypoint->points.size() << endl;
  // cout << "Pathpoint_Temp.size(): "           << Pathpoint_Temp->points.size() << endl;
  // int origin_pathpoint_size = Pathpoint_Temp->points.size();


  // //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // vector< geometry_msgs::Pose > Rviz_TrajectoryPose =  RvizPose_pointInterpolation( origin_pathpoint_size,
  //                                                                                   rotation_originWaypoint, 
  //                                                                                   Pathpoint_Temp);

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////

  URx_Pose_generation(  trajectory_point_size, 
                        X_Normal_Vector, 
                        Y_Normal_Vector, 
                        Z_Normal_Vector, 
                        PathPoint_Position_final,                   
                        Welding_Trajectory);

  cout << "Welding_Trajectory.size(): "  << Welding_Trajectory.size() << endl;

  return Rviz_TrajectoryPose;
}





