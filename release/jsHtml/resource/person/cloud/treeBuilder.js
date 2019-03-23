import cpMap from '../src/router/cp_map'
/**
 * 生成路由树
 * 按钮类型标注：add<type> modify<type> remove<type> query<type> view（这个在菜单管理中必须要因为如果没有菜单将会消失）
 * */
export default function (tcode) {
 //传入的该账号的权限树
  let routerTree = [];

  for (let index in tcode) {
    traverseTree(tcode[index], routerTree);
  }
  return routerTree
}

function traverseTree (tree, rtree) {
  if (tree.type == 1) {
  //判断是不是二级菜单

    let router = cpMap.get(tree.menuCode);

    for(let index in tree.emus) {
   //给元数据加入按钮

      let attr = tree.emus[index].menuCode.split(':').pop()
      router.meta[attr] = true
    }

    rtree.push(router);
    return;
  }

  if (tree.emus.length == 0) {
    rtree.push(cpMap.get(tree.menuCode));
    return;
  }
  let component = cpMap.get(tree.menuCode);
  if (tree.emus && tree.emus.length > 0) {
    let i = 0;
    for (i = 0; i < tree.emus.length; i++) {
      traverseTree(tree.emus[i], component.children);
    }
  }
  rtree.push(component);
}

let a = [
  {
    "id": 255,
    "name": "首页",
    "code": "10031001",
    "parentCode": "1003",
    "menuCode": "home",
    "type": 0,
    "icon": "",
    "order_num": 1,
    "emus": []
  },
  {
    "id": 256,
    "name": "工作台",
    "code": "10031002",
    "parentCode": "1003",
    "menuCode": "workbench",
    "type": 0,
    "icon": "",
    "order_num": 1,
    "emus": [
      {
        "id": 261,
        "name": "主页",
        "code": "100310021001",
        "parentCode": "10031002",
        "menuCode": "workbench:workbench",
        "type": 1,
        "icon": "",
        "order_num": 1
      },
      {
        "id": 262,
        "name": "操作日志",
        "code": "100310021002",
        "parentCode": "10031002",
        "menuCode": "workbench:operation",
        "type": 1,
        "icon": "",
        "order_num": null
      }
    ]
  },
  {
    "id": 260,
    "name": "系统管理",
    "code": "10031006",
    "parentCode": "1003",
    "menuCode": "systemManage",
    "type": 0,
    "icon": "",
    "order_num": 1,
    "emus": [
      {
        "id": 272,
        "name": "菜单管理",
        "code": "100310061005",
        "parentCode": "10031006",
        "menuCode": "systemManage:menuManage",
        "type": 1,
        "icon": "",
        "order_num": null
      }
    ]
  }
]
