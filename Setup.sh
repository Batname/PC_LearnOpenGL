include='include'

echo "Setup OpenGL"

echo "[INSTALL] glm"
glm="$include/glm"
glm_ver='0.9.8.4'
glm_ar="$glm_ver.tar.gz"
if [ ! -d $glm ]; then
    wget "https://github.com/g-truc/glm/archive/$glm_ar" -P $include
    tar -xf "$include/$glm_ar" -C $include
    mv "$include/glm-$glm_ver/glm" $include
    rm -rf "$include/glm-$glm_ver"
    rm "$include/$glm_ar"
fi
