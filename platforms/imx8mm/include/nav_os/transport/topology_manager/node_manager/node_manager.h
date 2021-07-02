#ifndef __TRANSPORT_TOPOLOGYMANAGER_H_
#define __TRANSPORT_TOPOLOGYMANAGER_H_

#include "transport/topology_manager/manager.h"
#include "transport/container/multi_value_warehouse.h"
namespace xag_nav{
namespace os{
namespace transport{


class NodeManager;
using NodeManagerPtr = std::shared_ptr<NodeManager>;
/**
 * @class NodeManager
 * @brief Topology Manager of Node related
 */
class NodeManager : public Manager {
  friend class TopologyManager;

 public:
  using RoleAttrVec = std::vector<RoleAttributes>;
  using NodeWarehouse = MultiValueWarehouse;

  /**
   * @brief Construct a new Node Manager object
   */
  NodeManager();

  /**
   * @brief Destroy the Node Manager object
   */
  virtual ~NodeManager();

  /**
   * @brief Checkout whether we have `node_name` in topology
   *
   * @param node_name Node's name we want to inquire
   * @return true if this node found
   * @return false if this node not exits
   */
  bool HasNode(const std::string& node_name);

  /**
   * @brief Get the Nodes object
   *
   * @param nodes result RoleAttr vector
   */
  void GetNodes(RoleAttrVec* nodes);

 private:
  bool Check(const RoleAttributes& attr) override;
  void Dispose(const ChangeMsg& msg) override;
  void OnTopoModuleLeave( int process_id) override;

  void DisposeJoin(const ChangeMsg& msg);
  void DisposeLeave(const ChangeMsg& msg);
  
  NodeWarehouse nodes_;
};



}
}
}


#endif