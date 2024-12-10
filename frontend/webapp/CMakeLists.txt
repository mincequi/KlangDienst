file(GLOB_RECURSE WEBAPP_ASSETS
  "build/web/*.bin"
  "build/web/*.frag"
  "build/web/*.html"
  "build/web/*.js"
  "build/web/*.json"
  "build/web/*.otf"
  "build/web/*.png"
)

cmrc_add_resource_library(webapp WHENCE build/web
  ${WEBAPP_ASSETS}
)
