new_git_repository(
    name = "googletest",
    build_file = "third_party/gtest/gmock.BUILD",
    remote = "https://github.com/google/googletest",
    tag = "release-1.8.0",
)

git_repository(
  name = "org_pubref_rules_protobuf",
  remote = "https://github.com/pubref/rules_protobuf",
  tag = "v0.8.2",
  #commit = "..." # alternatively, use latest commit on master
)
load("@org_pubref_rules_protobuf//cpp:rules.bzl", "cpp_proto_repositories")
cpp_proto_repositories()

new_http_archive(
  name = "zlib_archive",
  build_file = "third_party/zlib/zlib.BUILD",
  sha256 = "c3e5e9fdd5004dcb542feda5ee4f0ff0744628baf8ed2dd5d66f8ca1197cb1a1",
  strip_prefix = "zlib-1.2.11",
  urls = [
    "https://mirror.bazel.build/zlib.net/zlib-1.2.11.tar.gz",
    "https://zlib.net/zlib-1.2.11.tar.gz",
  ],
)

git_repository(
    name = "com_github_gflags_gflags",
    remote = "https://github.com/gflags/gflags.git",
    tag = "v2.2.2"
)
