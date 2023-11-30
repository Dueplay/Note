Git Tutorial 

### Git Local

#### 1.Check the Version

```shell
$ git --version
```

#### 2.Configure Git

我们通常会将 git 配置为我们在 github 上注册的用户名/电子邮件/密码。**global** 关键字会为系统中的每个版本库设置配置。我们可以去掉 **global** 关键字，只对当前仓库进行配置。

```shell
# 配置
$ git config --global user.name "Dueplay"
$ git config --global user.email "2289535823@qq.com"
$ git config --global user.password "your passwd"

# 查看配置
$ git config user.name
Dueplay
$ git config user.email
2289535823@qq.com
$ git conig user.password
hello123
```

#### 3. Initialize Git

创建一个新的 repo，并如下初始化 git 以跟踪一个文件夹：

```shell
$ mkdir myproject && cd myproject
$ git init
```

在myproject有个隐藏文件夹 .git ，这是 git 为我们存储所有跟踪信息的地方。

#### 4. Check Status

```shell
$ git status
On branch master

No commits yet

Untracked files:
  (use "git add <file>..." to include in what will be committed)
	index.html

nothing added to commit but untracked files present (use "git add" to track)
```

#### 5.暂存新的改变到工作区

在 git 中，每个文件都可能处于两个阶段中的一个：

已跟踪--git 会主动监控该文件的任何更改，并将其作为 repo 的一部分
未跟踪--虽然文件存在，但 git 会 "忽略 "它的存在和变化

``` shell
# 
$ git add index.html
# 
$ git restore --staged [filename]
```

#### 6.Commit Changes

```shell
$ git commit -m "First release of Git Helloworld Project"
```

可以跳过暂存阶段，一步到位地comiit变更。不建议这样做

```shell
git commit -a -m [commit message]
```

#### 7. Commit History

```shell
$ git log
# 精简log
$ git log --oneline
```

#### 8.Help

git有这么多的标志选项和命令

git help --all 会显示所有可用的 git 命令。

git [command] -help 会显示该命令可设置的所有标志，而 git [command] --help 则会打开该命令的手册。(单--和双--）

#### 9.Git Branch

创建新分支

```shell
$ git branch hello-world-image
```

查看这个仓库所有可用的分支

```shell
$ git branch
  hello-world-image
* master
```

如果我们切换master主分支，新快照就不是主分支的一部分。我们在另一个分支中的新更改不会影响主分支。

通过 checkout 命令从主分支转移到新创建的分支：

```shell
$ git checkout hello-world-image
Switched to branch 'hello-world-image'
```

创建和切换到新分支可一步完成

```shell
$ git checkout -b hello-world-image
```

#### 10.Merge Branch

假设我们对 hello-world-image 分支中的新开发非常满意，决定将其合并回主分支。

既然要合并到主分支，我们首先要确保自己是站在主分支上的：

```shell
$ git checkout master
Switched to branch 'master'

# 使用merge合并分支
$ git merge hello-world-image
Updating 2daa287..c31a2c0
Fast-forward
 img_hello_world.png | Bin 0 -> 48630 bytes
 1 file changed, 0 insertions(+), 0 deletions(-)
 create mode 100644 img_hello_world.png
```

我们看到 git 说这次合并是 "Fast-forward"，因为它没有发现这两个分支有任何冲突，合并进行得很顺利。

然而，生活并不总是一帆风顺。有时会发生合并冲突。

假设现在我们回到 hello-world-image，在 index.html 中添加一行新内容。同时，我们在主分支中删除 index.html 中的一行，并在两个分支中都提交更改。

现在，当我们尝试将此提交与主分支合并时，冲突就会发生：

```shell
$ git merge hello-world-image
Auto-merging index.html
CONFLICT (content): Merge conflict in index.html
Automatic merge failed; fix conflicts and then commit the result.
```

我们可以打开 index.html，看看 git 对冲突发生的原因和方式做了哪些标记：

```shell
<!DOCTYPE html>
<html>
<head>
<title>Hello World!</title>
<link rel="stylesheet" href="bluestyle.css">
</head>
<body>

<h1>Hello world!</h1>
<p>This is the first file in my new Git Repo.</p>
<<<<<<< HEAD
=======

<p>A old line in our file!</p>
>>>>>>> hello-world-image
</body>
</html>
```

我们可以选择保留 <<<<<<< HEAD 至 ======= 之间的所有内容，或者保留 ======= 至 >>>>>>> hello-world-image 之间的所有内容。

在对 index.html 中的冲突部分做出选择后，我们将文件暂存并提交，以结束这次合并操作。

```shell
$ git add index.html
$ git commit -m "after fix the conflict"
[master bdfc2b1] after fix the conflict
```

这样就完成了两个冲突分支的合并，通过 -d 标志，我们可以删除合并后的分支，以保持工作区的整洁：

```shell
$ git branch -d hello-world-images
Deleted branch hello-world-image (was 9e7a8ee).
```

#### 11.Revert

revert 命令用于删除之前的提交，并将删除内容变成新的提交，而不修改日志。

首先，我们要检查哪个提交是我们想要返回的：（使用 --oneline 关键字显示简洁的 git 日志）

```shell
$ git log --oneline
bdfc2b1 (HEAD -> master) fix conflicts:
9e7a8ee add a line
17fd1f5 remove a line
5ec2abb a small change
c31a2c0 Added image to Hello World
2daa287 Another changes
48d7a59 First release of Git Helloworld Project

$ git revert 9e7a8ee --no-edit
```



#### 12.Reset

reset 会将 repo 全部移回之前的提交，会删除该提交和最新版本之间的所有更改。

现在，假设我们添加了两个文件 file1.txt、file2.txt 和 file3.txt，并将它们包含在两个不同的提交中：

```shell
$ touch file1.txt file2.txt file3.txt
$ git add file1.txt && git commit -m "Add file1"
$ git add file2.txt && git commit -m "Add file2"
$ git add file3.txt && git commit -m "Add file3"
$ git log --oneline
327ae72 (HEAD -> master) Add file3
8b159b4 Add file2
cf9f3bf Add file1
...(more)...
```

现在我们不需要文件 2 和文件 3，但希望保留文件 1。我们可以分别revert最近的 2 次提交，或者reset回添加 file1 的提交。

```shell
$ git reset cf9f3bf
$ git log
cf9f3bf (HEAD -> master) Add file1
...(more)...
```

警告：通常情况下，乱动版本库的提交历史是很危险的，尤其是在与他人合作时。

如果我们知道 git 的提交哈希值，就有办法撤销reset。在前面的例子中，即使我们reset回 file1 的提交，我们仍然可以返回，因为我们知道上次添加文件 3 的提交哈希值是 327ae72。

```shell
$ git reset 327ae72
$ git log
327ae72 (HEAD -> master) Add file3
8b159b4 Add file2
cf9f3bf Add file1
...(more)...
```

#### 13.Amend

commit --amend 可用于修改最近的提交，并交换更改其提交信息。

它将暂存区域的改动与最新提交结合起来，并从中创建一个新提交，取代最新提交。

例如，我们对 README.md 稍作改动，想要提交，但在提交信息中输入了不少错别字。

```shell
$ git add README.md
$ git commit -m "Upated: RMEADE.md (ugly typos)"
$ git log --oneline
b0dfb07 (HEAD -> master) Upated: RMEADE.md (ugly typos)
327ae72 Add file3
...(more)...
```

我们的错别字让 git 历史看起来很糟糕。使用amend修改提交语句

```shell
$ git commit --amend -m "Update: README.md (beautiful)"
$ git log --oneline
d4bf700 (HEAD -> master) Update: README.md (beautiful)
327ae72 Add file3
...(more)...
```

### 2.Git Remote

首先需要创建一个新的 github 仓库，然后上传我们目前正在开发的本地仓库，以便同步。

为了方便起见，我们将远程仓库也命名为 myproject。

### 1.与远程仓库同步

在本地项目仓库中，我们需要添加一个远程跟踪链接：

```shell
git remote add origin https://github.com/Dueplay/myproject.git
```

通过 git remote -v 检查是否与 github 上的 repo 远程同步

```shell
$ git remote -v
origin	https://github.com/[username]/myproject.git (fetch)
origin	https://github.com/[username]/myproject.git (push)
```

然后，我们就可以将所有本地开发的内容推送到远程版本库：

**git push**：将本地更改推送到远程仓库的命令，完整格式 ：git push <remote> <branch>

- `<remote>` 是远程仓库的名称，通常是 "origin" 或其他你设置的别名。
- `<branch>` 是本地分支的名称，它将被推送到远程仓库的同名分支。

**--set-upstream**（或者简写为 -u）: 这部分告诉Git为本地分支与远程分支建立关联关系。关联关系的作用是在将来的推送或拉取操作中，Git会自动识别要使用的远程分支。设置关联关系后，你就可以使用`git push`和`git pull`而不需要每次都指定远程仓库和分支,只需输入 `git push`，Git 将自动使用关联的远程和分支。

```shell
$ git push --set-upstream origin master
 
Enumerating objects: 34, done.
Counting objects: 100% (34/34), done.
Delta compression using up to 8 threads
Compressing objects: 100% (33/33), done.
Writing objects: 100% (34/34), 49.37 KiB | 12.34 MiB/s, done.
Total 34 (delta 17), reused 0 (delta 0), pack-reused 0
remote: Resolving deltas: 100% (17/17), done.
To https://github.com/[username]/myproject.git
 * [new branch]      master -> master
Branch 'master' set up to track remote branch 'master' from 'origin'.
/*你的本地 master 分支现在已设置为跟踪远程仓库 'origin' 上的远程 master 分支*/
```

现在，我们已将本地所有开发成果上传到远程版本库

### 2.拉取远程仓库新东西到本地

现在，假设远程 repo 上出现了新情况。在此，我们将模拟这种情况，直接从 github 上的 README.md 中移除一行并提交。

我们如何更新本地版本库以包含这些更改呢？

我们有两个选择：

- `fetch` + `merge`
- `pull` 

从第一个组合开始。我们首先需要使用 fetch 从远程下载所有新更改

```shell
$ git fetch origin
remote: Enumerating objects: 5, done.
remote: Counting objects: 100% (5/5), done.
remote: Compressing objects: 100% (1/1), done.
remote: Total 3 (delta 2), reused 2 (delta 2), pack-reused 0
Unpacking objects: 100% (3/3), 689 bytes | 344.00 KiB/s, done.
From https://github.com/[username]/myproject
   d4bf700..6d4ad42  master     -> origin/master
   
# 现在我们有了来自远程的新改动，可以检查一下 git 状态了：
$ git status
On branch master
Your branch is behind 'origin/master' by 1 commit, and can be fast-forwarded.
  (use "git pull" to update your local branch)
```

我们看到本地仓库比上游主仓库晚提交了 1 次，即 README.md 上的一行改动。我们可以使用 git diff 命令仔细检查具体的差异

```shell
$ git diff origin/master
diff --git a/README.md b/README.md
index f3fa9a9..cf28200 100644
--- a/README.md
+++ b/README.md
@@ -3,3 +3,5 @@ Hello World repository for Git tutorial
 This is an example repository for the Git tutoial on https://www.w3schools.com
 
 This repository is built step by step in the tutorial.
+
+a new line
```

合并提交

```shell
$ git merge origin/master
Updating d4bf700..6d4ad42
Fast-forward
 README.md | 2 --
 1 file changed, 2 deletions(-)

$ git status
On branch master
Your branch is up to date with 'origin/master'.
```

现在我们的本地 git 已经是最新的了。

上述方法可行，但有点繁琐。其实我们可以用 pull 命令一步完成更新，它是 fetch 和 merge 的结合。

让我们把从远程 github 上的 README.md 中删除的新行添加回去，并尝试把更改拉入本地 git。

```shell
$ git pull origin
remote: Enumerating objects: 5, done.
remote: Counting objects: 100% (5/5), done.
remote: Compressing objects: 100% (3/3), done.
remote: Total 3 (delta 2), reused 0 (delta 0), pack-reused 0
Unpacking objects: 100% (3/3), 696 bytes | 232.00 KiB/s, done.
From https://github.com/[username]/myproject
   6d4ad42..b2bb9ae  master     -> origin/master
Updating 6d4ad42..b2bb9ae
Fast-forward
 README.md | 2 ++
 1 file changed, 2 insertions(+)
```

可以看到我们已经更新了远程主分支：

```shell
$ git log --oneline
b2bb9ae (HEAD -> master, origin/master) Add back a new line from github directly
6d4ad42 Remove the new line README.md  GitHub directly
d4bf700 Update: README.md (beautiful)
327ae72 Add file3
8b159b4 Add file2
cf9f3bf Add file1
...(more)...
```

### 3.推送本地新修改到远程

现在，我们将通过 push 命令为远程仓库做出贡献。

让我们在 README.md 中再添加一行，然后尝试更新远程版本库。

```shell
$ git commit -a -m "Update readme locally and try push"
[master f8986b8] Update readme locally and try push
 1 file changed, 1 insertion(+)

$ git status
On branch master
Your branch is ahead of 'origin/master' by 1 commit.
  (use "git push" to publish your local commits)
```

Git 显示我们比远程主分支早提交 1 次。让我们把更改推送到远程仓库。

```shell
$ git push origin
```

### 4.Remote Branch Pull

可以通过github图形用户界面直接在远程仓库创建一个新分支。我们从主分支创建了一个名为 secondary 的新分支，并对 README.md 做了一些修改，直接在 github 上提交。

我们可以将新分支拉到本地 git 并检查它：

```shell
$ git pull
remote: Enumerating objects: 5, done.
remote: Counting objects: 100% (5/5), done.
remote: Compressing objects: 100% (3/3), done.
remote: Total 3 (delta 2), reused 0 (delta 0), pack-reused 0
Unpacking objects: 100% (3/3), 697 bytes | 232.00 KiB/s, done.
From https://github.com/[username]/myproject
 * [new branch]      secondary  -> origin/secondary
Already up to date.
```

我们可以像往常一样通过 git 分支查看新分支。但默认情况下，它只显示本地分支。我们需要使用 -a 标志来查看所有本地和远程分支，或者使用 -r 标志来查看远程分支。

```shell
$ git branch // only local branches
* master

$ git branch -a
* master
  remotes/origin/master
  remotes/origin/secondary

$ git branch -r 
  origin/master
  origin/secondary
```

### 5.将本地分支推送到远程
我们也可以将本地新分支的变更推送到远程 repo。

```shell
$ git checkout -b local-new-branch

...
(do some changes to README.md)
...

$ git commit -a -m "Update from local-new-branch"
[local-new-branch 40e9ee3] Update: local-new-branch
 1 file changed, 2 insertions(+)
 
$ git push origin local-new-brancch
Enumerating objects: 5, done.
Counting objects: 100% (5/5), done.
Delta compression using up to 8 threads
Compressing objects: 100% (3/3), done.
Writing objects: 100% (3/3), 334 bytes | 334.00 KiB/s, done.
Total 3 (delta 2), reused 0 (delta 0), pack-reused 0
remote: Resolving deltas: 100% (2/2), completed with 2 local objects.
remote: 
remote: Create a pull request for 'local-new-branch' on GitHub by visiting:
remote:      https://github.com/[username]/myproject/pull/new/local-new-branch
remote: 
To https://github.com/[username]/myproject.git
 * [new branch]      local-new-branch -> local-new-branch
```

现在，如果我们访问 github 远程仓库，就会发现有一个新推送的分支，名为 local-new-branch

### 6.Merge Into Master

如上所述，在 github repo 页面上，我们希望将本地-新分支中的更改合并到主分支中。网页上有 " **Compare & Pull Request**."选项。

我们可以点击它并创建一个**pull request**请求。

因为这是我们自己的 repo，所以我们是 "权威"，可以直接点击 "**Merge pull request**  "将更改合并到主分支。

但在现实生活中，拉取请求通常需要经过代码审查和测试流程，并由合作者验证。如果获得批准，拉取请求才会通过并被合并。

### 7.clone and fork

```shell
$ git clone [the repo url] [the folder path we want to clone into]
```

### 8.Git Ignore

我们并不一定希望 git 追踪本地仓库中的每一个文件。可能有一些日志文件、临时文件或个人文件不应该被纳入 git 工作流。

为了解决这个问题，我们可以在 git 仓库中创建一个 .gitignore 文件，这样 git 就会忽略其中指定的文件。不过，.gitignore 文件本身会被 git 追踪。

常见的用法包括

| command     | Description                               |
| ----------- | ----------------------------------------- |
|             | blank lines are ignored                   |
| # something | lines start with # is comment and ignored |
| *name*      | All *name* files and folders              |
| *name/*     | All folders called *name*                 |
| *name.file* | All *name.file* in repo                   |
| **.file*    | any file with extension *.file*           |
| *!\*.file*  | negate any previous ignore on this file   |