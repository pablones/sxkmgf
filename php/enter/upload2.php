 <?php\
\
\
\
    $userfile = $HTTP_POST_FILES['userfile']['tmp_name'];\
\
\
\
\
    $userfile_name = $HTTP_POST_FILES['userfile']['name'];\
\
\
\
\
    $userfile_size = $HTTP_POST_FILES['userfile']['size'];\
\
\
\
    $userfile_type = $HTTP_POST_FILES['userfile']['type'];\
\
\
\
    $userfile_error = $HTTP_POST_FILES['userfile']['error'];\
\
\
    if ($userfile_error > 0) {\
\
\
    echo 'Problem: ';\
\
\
    switch ($userfile_error)\
\
\
    { case 1:\
\
\
    echo 'File exceeded upload_max_filesize';\
\
\
    break;\
\
\
    case 2:\
\
\
    echo 'File exceeded max_file_size';\
\
\
    break;\
\
\
    case 3:\
\
\
    echo 'File only partially uploaded';\
\
\
    break;\
\
\
    case 4:\
\
\
    echo 'No file uploaded';\
\
\
    break;\
\
\
    }\
\
\
    exit;\
\
\
    }\
\
\
    $upfile = '/'.$userfile_name;\
\
\
    if (is_uploaded_file($userfile))\
\
\
    {\
\
\
    if (!move_uploaded_file($userfile, $upfile))\
\
\
    {\
\
\
    echo 'Problem: Could not move file to destination directory';\
\
\
    exit;\
\
\
    }\
\
\
    } else {\
\
\
    echo 'Problem: Possible file upload attack. Filename: '.$userfile_name;\
\
\
    exit;\
\
\
    }\
\
\
    echo 'File uploaded successfully<br /><br />';\
\
    echo 'Preview of uploaded file contents:<br /><hr />';\
\
\
    echo $contents;\
\
\
    echo '<br /><hr />';\
\
\
    ?>
